#pragma once
#include "esphome.h"
#include <esp_heap_caps.h>
#include <lvgl.h>
<<<<<<< Updated upstream
#include <ctime> // Required for the filename timestamp

using namespace esphome::web_server_idf;

// This class handles web requests to capture and serve a screenshot
class ScreenshotHandler : public AsyncWebHandler {
 public:
  // canHandle determines if this script should respond to a specific URL request
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buf[513];
    auto url = request->url_to(buf);
    // Only respond to GET requests for /screenshot or /screenshot.bmp
    return request->method() == HTTP_GET &&
           (url == "/screenshot.bmp" || url == "/screenshot");
=======
#include <ctime>
#include <inttypes.h>
#include "esp_jpeg_enc.h"

using namespace esphome::web_server_idf;

static uint16_t *g_framebuffer = nullptr;
static int32_t g_fb_width = 0;
static int32_t g_fb_height = 0;

void screenshot_capture_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint16_t *px_map) {
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

void screenshot_init(int32_t width, int32_t height) {
  g_fb_width  = width;
  g_fb_height = height;
  size_t needed = (size_t)width * height * 2;
  g_framebuffer = (uint16_t *) heap_caps_calloc(needed, 1, MALLOC_CAP_SPIRAM);
  if (g_framebuffer) {
    ESP_LOGI("screenshot", "Framebuffer allocated: %" PRId32 "x%" PRId32 " (%zu bytes), free PSRAM: %zu",
             width, height, needed, heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  } else {
    ESP_LOGE("screenshot", "Framebuffer allocation failed! Free PSRAM: %zu",
             heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  }
}

class ScreenshotHandler : public AsyncWebHandler {
 public:
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buf[513];
    auto url = request->url_to(buf);
    return request->method() == HTTP_GET &&
           (url == "/screenshot.jpg" || url == "/screenshot");
>>>>>>> Stashed changes
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    char buf[513];
    auto url = request->url_to(buf);

<<<<<<< Updated upstream
    // PART 1: The HTML Interface (Preview page)
=======
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        "#dl{background:#4CAF50; display:none;}" // Download button hidden by default
=======
        "#dl{background:#4CAF50;display:none;}"
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        "function refresh() {"
        "  status.textContent='Generating...';"
        "  var t=new Date();"
        "  var next=new Image();"
        "  next.onload=function() {"
        "    img.src=next.src;"
        "    img.style.display='block';"
        "    dl.href=next.src;" // Update the download link to the new image
        "    dl.style.display='inline-block';" // Show the download button
        "    status.textContent='Screenshot from '+t.toLocaleTimeString();"
        "  };"
        "  next.onerror=function() {"
        "    status.textContent='Error loading image, please try again.';"
        "  };"
        "  next.src='/screenshot.bmp?t='+t.getTime();" // Cache-busting parameter
=======
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
        "  next.src='/screenshot.jpg?t='+t.getTime();"
>>>>>>> Stashed changes
        "}"
        "</script>"
        "</body></html>";
      request->send(200, "text/html", html);
      return;
    }

<<<<<<< Updated upstream
    // PART 2: Generating the BMP file
    lv_display_t *disp = lv_display_get_default();
    if (!disp) { request->send(500, "text/plain", "No display found"); return; }

    int32_t width  = lv_display_get_horizontal_resolution(disp);
    int32_t height = lv_display_get_vertical_resolution(disp);

    // Get the active draw buffer from LVGL
    lv_draw_buf_t *draw_buf = lv_display_get_buf_active(disp);
    if (!draw_buf || !draw_buf->data) {
      request->send(500, "text/plain", "No framebuffer available");
      return;
    }

    // BMP calculations: rows must be a multiple of 4 bytes (padding)
    uint32_t row_size   = ((width * 3 + 3) / 4) * 4;
    uint32_t pixel_data = row_size * height;
    uint32_t file_size  = 54 + pixel_data; // 54 bytes is the standard BMP header

    // Allocate memory in PSRAM (SPIRAM) because internal RAM is usually too small
    uint8_t *bmp = (uint8_t *) heap_caps_malloc(file_size, MALLOC_CAP_SPIRAM);
    if (!bmp) { request->send(503, "text/plain", "Not enough PSRAM available"); return; }

    // Manually fill the BMP Header
    memset(bmp, 0, 54);
    bmp[0] = 'B'; bmp[1] = 'M'; // File type
    uint32_t fs     = file_size;
    uint32_t offset = 54;
    uint32_t pd     = pixel_data;
    uint32_t dib    = 40;
    int32_t  w      = width;
    int32_t  h      = -height; // Negative height ensures top-down pixel order
    uint16_t planes = 1;
    uint16_t bpp    = 24;      // 24-bit color (RGB888)

    memcpy(bmp + 2,  &fs,     4);
    memcpy(bmp + 10, &offset, 4);
    memcpy(bmp + 14, &dib,    4);
    memcpy(bmp + 18, &w,      4);
    memcpy(bmp + 22, &h,      4);
    memcpy(bmp + 26, &planes, 2);
    memcpy(bmp + 28, &bpp,    2);
    memcpy(bmp + 34, &pd,     4);

    // Copy and convert pixels from LVGL (RGB565) to BMP (RGB888)
    // In LVGL v9 the framebuffer is raw RGB565 uint16_t data
    uint16_t *fb = (uint16_t *) draw_buf->data;
    uint8_t *dst = bmp + 54;

    for (int32_t y = 0; y < height; y++) {
      uint8_t *row = dst + y * row_size;
      for (int32_t x = 0; x < width; x++) {
        uint16_t pixel = fb[y * width + x];
        // RGB565: RRRRRGGGGGGBBBBB -> scale each channel to 8-bit
        uint8_t r = (pixel >> 11) & 0x1F;
        uint8_t g = (pixel >> 5)  & 0x3F;
        uint8_t b =  pixel        & 0x1F;
        row[x * 3 + 2] = (r << 3) | (r >> 2); // Red
        row[x * 3 + 1] = (g << 2) | (g >> 4); // Green
        row[x * 3 + 0] = (b << 3) | (b >> 2); // Blue
      }
    }

    // PART 3: Generate dynamic filename with timestamp
    time_t now = ::time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    char filename[64];
    // Format: OpenAmber_Screenshot_YYYYMMDDHHMM.bmp
    strftime(filename, sizeof(filename), "OpenAmber_Screenshot_%Y%m%d%H%M.bmp", &timeinfo);

    // Prepare header for download with the unique filename
    char header_content[128];
    snprintf(header_content, sizeof(header_content), "attachment; filename=\"%s\"", filename);

    // Send the BMP file to the browser
    auto *response = request->beginResponse(200, "image/bmp", bmp, file_size);
    response->addHeader("Content-Disposition", header_content);
    request->send(response);

    // Free memory after sending to prevent leaks
    heap_caps_free(bmp);
  }
};
=======
    if (!g_framebuffer) {
      request->send(503, "text/plain", "Framebuffer not initialized");
      return;
    }

    // Configure JPEG encoder (esp_new_jpeg v1.0.0 API)
    jpeg_enc_config_t enc_cfg = DEFAULT_JPEG_ENC_CONFIG();
    enc_cfg.width       = g_fb_width;
    enc_cfg.height      = g_fb_height;
    enc_cfg.src_type    = JPEG_PIXEL_FORMAT_RGB565_LE;
    enc_cfg.subsampling = JPEG_SUBSAMPLE_420;
    enc_cfg.quality     = 80;
    enc_cfg.task_enable = false;

    jpeg_enc_handle_t enc_handle = NULL;
    if (jpeg_enc_open(&enc_cfg, &enc_handle) != JPEG_ERR_OK) {
      request->send(500, "text/plain", "JPEG encoder init failed");
      return;
    }

    // Allocate output buffer (~300KB should be more than enough for quality 80)
    size_t jpg_buf_size = 300 * 1024;
    uint8_t *jpg_buf = (uint8_t *) heap_caps_malloc(jpg_buf_size, MALLOC_CAP_SPIRAM);
    if (!jpg_buf) {
      jpeg_enc_close(enc_handle);
      request->send(503, "text/plain", "Not enough PSRAM for JPEG buffer");
      return;
    }

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

    time_t now = ::time(nullptr);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    char filename[64];
    strftime(filename, sizeof(filename), "OpenAmber_Screenshot_%Y%m%d%H%M.jpg", &timeinfo);
    char header_content[128];
    snprintf(header_content, sizeof(header_content), "attachment; filename=\"%s\"", filename);

    auto *response = request->beginResponse(200, "image/jpeg", jpg_buf, out_len);
    response->addHeader("Content-Disposition", header_content);
    request->send(response);

    heap_caps_free(jpg_buf);
  }
};

inline void register_screenshot_handler() {
  static ScreenshotHandler handler;
  auto *base = esphome::web_server_base::global_web_server_base;
  if (base == nullptr) { ESP_LOGE("screenshot", "web_server_base is null"); return; }
  auto *server = base->get_server();
  if (server == nullptr) { ESP_LOGE("screenshot", "AsyncWebServer is null"); return; }
  server->addHandler(&handler);
  ESP_LOGI("screenshot", "Screenshot handler registered");
}
>>>>>>> Stashed changes
