/**
 * @file screenshot.h
 * @brief Screenshot handler voor OpenAmber — serveert een live JPEG afbeelding
 *        van het LVGL display via HTTP.
 *
 * Gebruik:
 *   - GET /screenshot      → HTML preview pagina met knop om screenshot te maken
 *   - GET /screenshot.jpg  → JPEG afbeelding van het huidige scherm
 *
 * Werking:
 *   De LVGL flush callback stuurt elke gerenderde tegel door naar een globale
 *   framebuffer in PSRAM. Bij een HTTP-verzoek wordt die buffer via de
 *   esp_new_jpeg encoder gecomprimeerd en als JPEG teruggestuurd.
 *
 * Vereisten:
 *   - esp_new_jpeg >= 1.0.0 (via idf_component.yml of ESPHome components)
 *   - PSRAM aanwezig (minimaal ~1.5 MB vrij voor framebuffer + JPEG buffer)
 *   - Aanroep van screenshot_init() en register_screenshot_handler() bij boot
 *   - Aanroep van screenshot_capture_flush() vanuit de LVGL flush event callback
 */

#pragma once
#include "esphome.h"
#include <esp_heap_caps.h>
#include <lvgl.h>
#include <ctime>
#include <inttypes.h>
#include "esp_jpeg_enc.h"

using namespace esphome::web_server_idf;

// ---------------------------------------------------------------------------
// Globale framebuffer — gevuld door screenshot_capture_flush()
// Formaat: RGB565, 2 bytes per pixel, volledige schermresolutie
// ---------------------------------------------------------------------------
static uint16_t *g_framebuffer = nullptr;
static int32_t   g_fb_width    = 0;
static int32_t   g_fb_height   = 0;

/**
 * @brief Kopieert een LVGL render-tegel naar de globale framebuffer.
 *
 * Aanroepen vanuit de LVGL LV_EVENT_FLUSH_START callback, zodat de
 * framebuffer altijd de meest recente scherminhoud bevat.
 *
 * @param x1     Linkerkolom van de tegel
 * @param y1     Bovenste rij van de tegel
 * @param x2     Rechterkolom van de tegel (inclusief)
 * @param y2     Onderste rij van de tegel (inclusief)
 * @param px_map RGB565 pixeldata van de tegel
 */
void screenshot_capture_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                               const uint16_t *px_map) {
  if (!g_framebuffer || !px_map) return;
  int32_t src_w = x2 - x1 + 1;
  for (int32_t y = y1; y <= y2; y++) {
    if (y >= g_fb_height) continue;
    for (int32_t x = x1; x <= x2; x++) {
      if (x >= g_fb_width) continue;
      g_framebuffer[y * g_fb_width + x] = px_map[(y - y1) * src_w + (x - x1)];
    }
  }
}

/**
 * @brief Alloceert de globale framebuffer in PSRAM.
 *
 * Aanroepen bij boot, vóór register_screenshot_handler().
 * De buffer heeft width * height * 2 bytes nodig (RGB565).
 *
 * @param width  Horizontale schermresolutie in pixels
 * @param height Verticale schermresolutie in pixels
 */
void screenshot_init(int32_t width, int32_t height) {
  g_fb_width  = width;
  g_fb_height = height;
  size_t needed = (size_t)width * height * 2;
  g_framebuffer = (uint16_t *) heap_caps_calloc(needed, 1, MALLOC_CAP_SPIRAM);
  if (g_framebuffer) {
    ESP_LOGI("screenshot", "Framebuffer allocated: %" PRId32 "x%" PRId32
             " (%zu bytes), free PSRAM: %zu",
             width, height, needed, heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  } else {
    ESP_LOGE("screenshot", "Framebuffer allocation failed! Free PSRAM: %zu",
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  }
}

// ---------------------------------------------------------------------------
// HTTP handler
// ---------------------------------------------------------------------------

/**
 * @brief AsyncWebHandler die /screenshot en /screenshot.jpg afhandelt.
 *
 * Registreer via register_screenshot_handler() — niet direct instantiëren.
 */
class ScreenshotHandler : public AsyncWebHandler {
 public:

  /**
   * @brief Bepaalt of deze handler het verzoek afhandelt.
   *
   * Reageert uitsluitend op GET-verzoeken naar /screenshot of /screenshot.jpg.
   */
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buf[513];
    auto url = request->url_to(buf);
    return request->method() == HTTP_GET &&
           (url == "/screenshot.jpg" || url == "/screenshot");
  }

  /**
   * @brief Verwerkt het HTTP-verzoek.
   *
   * /screenshot      → stuurt een HTML-pagina met preview en downloadknop.
   * /screenshot.jpg  → codeert de framebuffer als JPEG en stuurt die terug.
   */
  void handleRequest(AsyncWebServerRequest *request) override {
    char buf[513];
    auto url = request->url_to(buf);

    // --- HTML preview pagina ---
    if (url == "/screenshot") {
      const char* html =
        "<html><head><meta name='viewport' content='width=device-width'>"
        "<title>OpenAmber Live</title>"
        "<style>"
        "body{font-family:sans-serif;margin:0;background:#111;color:white;text-align:center;}"
        "h2{padding:0.5em;margin:0;background:#1976d2;}"
        "img{max-width:100%;height:auto;display:block;margin:0 auto;border:2px solid #333;}"
        "#status{font-size:0.85em;padding:0.4em;color:#aaa;}"
        "button,a{display:inline-block;margin:0.5em;padding:0.5em 1.5em;background:#1976d2;"
        "color:white;text-decoration:none;border-radius:6px;border:none;font-size:1em;cursor:pointer;}"
        "button:active{background:#1256a0;}"
        "#dl{background:#4CAF50;display:none;}"
        "</style></head>"
        "<body>"
        "<h2>OpenAmber Preview</h2>"
        "<div id='status'>Press the button to take a screenshot.</div>"
        "<br>"
        "<button onclick='refresh()'>&#128247; Take Screenshot</button>"
        "<button onclick='location.reload()'>&#8635; Reload Page</button>"
        "<a id='dl' href='#' download>&#11015; Download Screenshot</a>"
        "<br><br>"
        "<img id='preview' src='' style='display:none'>"
        "<script>"
        "var img=document.getElementById('preview');"
        "var status=document.getElementById('status');"
        "var dl=document.getElementById('dl');"
        "function refresh(){"
        "  status.textContent='Generating...';"
        "  var t=new Date();"
        "  var next=new Image();"
        "  next.onload=function(){"
        "    img.src=next.src;"
        "    img.style.display='block';"
        "    dl.href=next.src;"
        "    dl.style.display='inline-block';"
        "    status.textContent='Screenshot from '+t.toLocaleTimeString();"
        "  };"
        "  next.onerror=function(){"
        "    status.textContent='Error loading image, please try again.';"
        "  };"
        "  next.src='/screenshot.jpg?t='+t.getTime();" // Cache-busting via timestamp
        "}"
        "</script>"
        "</body></html>";
      request->send(200, "text/html", html);
      return;
    }

    // --- JPEG screenshot ---
    if (!g_framebuffer) {
      request->send(503, "text/plain", "Framebuffer not initialized");
      return;
    }

    // Configureer de JPEG encoder (esp_new_jpeg v1.0.0 API)
    jpeg_enc_config_t enc_cfg = DEFAULT_JPEG_ENC_CONFIG();
    enc_cfg.width       = g_fb_width;
    enc_cfg.height      = g_fb_height;
    enc_cfg.src_type    = JPEG_PIXEL_FORMAT_RGB565_LE;  // Invoerformaat van LVGL framebuffer
    enc_cfg.subsampling = JPEG_SUBSAMPLE_420;            // Kleursubsampling voor kleinere bestanden
    enc_cfg.quality     = 80;                            // JPEG kwaliteit (0-100)
    enc_cfg.task_enable = false;                         // Synchroon encoderen, geen aparte taak

    jpeg_enc_handle_t enc_handle = NULL;
    if (jpeg_enc_open(&enc_cfg, &enc_handle) != JPEG_ERR_OK) {
      request->send(500, "text/plain", "JPEG encoder init failed");
      return;
    }

    // Alloceer outputbuffer in PSRAM — 300 KB is ruim voldoende bij quality 80
    size_t jpg_buf_size = 300 * 1024;
    uint8_t *jpg_buf = (uint8_t *) heap_caps_malloc(jpg_buf_size, MALLOC_CAP_SPIRAM);
    if (!jpg_buf) {
      jpeg_enc_close(enc_handle);
      request->send(503, "text/plain", "Not enough PSRAM for JPEG buffer");
      return;
    }

    // Codeer de framebuffer naar JPEG
    int out_len = 0;
    jpeg_error_t err = jpeg_enc_process(enc_handle,
                                        (uint8_t *) g_framebuffer,
                                        (int)(g_fb_width * g_fb_height * 2),
                                        jpg_buf,
                                        (int)jpg_buf_size,
                                        &out_len);
    jpeg_enc_close(enc_handle);

    if (err != JPEG_ERR_OK || out_len <= 0) {
      heap_caps_free(jpg_buf);
      request->send(500, "text/plain", "JPEG encoding failed");
      return;
    }

    ESP_LOGI("screenshot", "JPEG encoded: %d bytes (%.1f KB)", out_len, out_len / 1024.0f);

    // Genereer een unieke bestandsnaam op basis van de huidige tijd
    time_t now = ::time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    char filename[64];
    strftime(filename, sizeof(filename), "OpenAmber_Screenshot_%Y%m%d%H%M.jpg", &timeinfo);

    char header_content[128];
    snprintf(header_content, sizeof(header_content), "attachment; filename=\"%s\"", filename);

    // Verstuur de JPEG als downloadbaar bestand
    auto *response = request->beginResponse(200, "image/jpeg", jpg_buf, out_len);
    response->addHeader("Content-Disposition", header_content);
    request->send(response);

    heap_caps_free(jpg_buf);
  }
};

// ---------------------------------------------------------------------------
// Registratie
// ---------------------------------------------------------------------------

/**
 * @brief Registreert de ScreenshotHandler op de ESPHome AsyncWebServer.
 *
 * Aanroepen vanuit on_boot (priority 200), nadat de webserver is gestart.
 * Combineert handler-registratie met de screenshot_init() aanroep.
 *
 * Voorbeeld in openamber-waveshare-display.yaml:
 * @code
 *   on_boot:
 *     - priority: 200
 *       then:
 *         - lambda: |-
 *             screenshot_init(1024, 600);
 *             register_screenshot_handler();
 * @endcode
 */
inline void register_screenshot_handler() {
  static ScreenshotHandler handler;
  auto *base = esphome::web_server_base::global_web_server_base;
  if (base == nullptr) {
    ESP_LOGE("screenshot", "web_server_base is null — handler niet geregistreerd");
    return;
  }
  auto *server = base->get_server();
  if (server == nullptr) {
    ESP_LOGE("screenshot", "AsyncWebServer is null — handler niet geregistreerd");
    return;
  }
  server->addHandler(&handler);
  ESP_LOGI("screenshot", "Screenshot handler geregistreerd op /screenshot en /screenshot.jpg");
}
