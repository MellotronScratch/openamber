// translations.h
#pragma once
#include <string>

// Enum voor alle vertaalbare strings
enum LangKey {
  // Boot screen
  LANG_BOOT_SUBTITLE,

  // Nav bar
  LANG_NAV_HOME,
  LANG_NAV_GRAPH,
  LANG_NAV_SETTINGS,
  LANG_NAV_SERVICE,

  // Page Home
  LANG_HOME_WATER_CIRCUIT,
  LANG_HOME_SUPPLY,
  LANG_HOME_RETURN,
  LANG_HOME_OUTSIDE,
  LANG_HOME_CURRENT_TARGET,
  LANG_HOME_FAN,
  LANG_HOME_PUMP,
  LANG_HOME_DEFROST,
  LANG_HOME_COMPRESSOR,
  LANG_HOME_BACKUP_HEATER,
  LANG_HOME_FLOOR_HEATING,
  LANG_HOME_HEAT_MODE,

  // Page Settings - sidebar
  LANG_STAB_GENERAL,
  LANG_STAB_FROST,
  LANG_STAB_PUMP,
  LANG_STAB_HEATING,
  LANG_STAB_COOLING,
  LANG_STAB_DHW,
  LANG_STAB_BIJVERWARMEN,
  LANG_STAB_SMARTGRID,
  LANG_STAB_WIFI,

  // Page Settings - labels
  LANG_SET_THERMOSTAT_SOURCE,
  LANG_SET_THERMOSTAT_SOURCE_DESC,
  LANG_SET_DHW_PRESENT,
  LANG_SET_DHW_PRESENT_DESC,
  LANG_SET_ANALYTICS,
  LANG_SET_ANALYTICS_DESC,
  LANG_SET_ANALYTICS_ID,
  LANG_SET_ANALYTICS_UNKNOWN,
  LANG_SET_FROST1_TITLE,
  LANG_SET_FROST1_DESC,
  LANG_SET_FROST2_TA_TITLE,
  LANG_SET_FROST2_TA_DESC,
  LANG_SET_FROST2_TUI_TITLE,
  LANG_SET_FROST2_TUI_DESC,
  LANG_SET_PUMP_P1,
  LANG_SET_PUMP_P1_DESC,
  LANG_SET_PUMP_HEAT,
  LANG_SET_PUMP_HEAT_DESC,
  LANG_SET_PUMP_DHW,
  LANG_SET_PUMP_DHW_DESC,
  LANG_SET_PUMP_INTERVAL,
  LANG_SET_PUMP_INTERVAL_DESC,

  // Page Service
  LANG_SVC_CHECKING_UPDATE,
  LANG_SVC_UPDATE_BTN,
  LANG_SVC_CHECK_UPDATES_BTN,
  LANG_SVC_FETCHING,
  LANG_SVC_QR_HINT,
  LANG_SVC_CONFIRM_TITLE,
  LANG_SVC_CONFIRM_DESC,
  LANG_SVC_CANCEL,
  LANG_SVC_START_UPDATE,
  LANG_SVC_INSTALLING,

  // Update loop (dynamisch opgebouwd, maar vaste prefix/suffix)
  LANG_UPDATE_AVAILABLE_PREFIX,   // "Beschikbaar: "
  LANG_UPDATE_CURRENT_PREFIX,     // " (huidig: "
  LANG_UPDATE_NONE,               // "Geen nieuwe software beschikbaar\nHuidige versie: "
  LANG_UPDATE_UNKNOWN,            // "onbekend"
  LANG_UPDATE_NO_SERVER,          // "Geen verbinding mogelijk met update server"
};

// Vertaaltabel
static const char* tr(int lang, LangKey key) {
  // lang: 0=NL, 1=EN, 2=DE

  static const char* strings[][3] = {
    // LANG_BOOT_SUBTITLE
    { "Verbinding maken met de warmtepomp...",
      "Connecting to heatpump...",
      "Verbinde mit Wärmepumpe..." },

    // LANG_NAV_HOME
    { "Home", "Home", "Home" },
    // LANG_NAV_GRAPH
    { "Grafiek", "Graph", "Diagramm" },
    // LANG_NAV_SETTINGS
    { "Instellingen", "Settings", "Einstellungen" },
    // LANG_NAV_SERVICE
    { "Service", "Service", "Service" },

    // LANG_HOME_WATER_CIRCUIT
    { "Water circuit", "Water circuit", "Wasserkreislauf" },
    // LANG_HOME_SUPPLY
    { "Aanvoer", "Supply", "Vorlauf" },
    // LANG_HOME_RETURN
    { "Retour", "Return", "Rücklauf" },
    // LANG_HOME_OUTSIDE
    { "Buiten", "Outside", "Außen" },
    // LANG_HOME_CURRENT_TARGET
    { "Huidig / Doel", "Current / Target", "Aktuell / Soll" },
    // LANG_HOME_FAN
    { "Fan", "Fan", "Lüfter" },
    // LANG_HOME_PUMP
    { "Pomp", "Pump", "Pumpe" },
    // LANG_HOME_DEFROST
    { "Defrost", "Defrost", "Abtauen" },
    // LANG_HOME_COMPRESSOR
    { "Compressor", "Compressor", "Kompressor" },
    // LANG_HOME_BACKUP_HEATER
    { "Backup heater", "Backup heater", "Heizstab" },
    // LANG_HOME_FLOOR_HEATING
    { "Bodemplaat\nverwarming", "Floor\nheating", "Bodenplatte\nheizung" },
    // LANG_HOME_HEAT_MODE
    { "Verwarmen", "Heating", "Heizen" },

    // LANG_STAB_GENERAL
    { "Algemeen", "General", "Allgemein" },
    // LANG_STAB_FROST
    { "Vorstbescherming", "Frost protection", "Frostschutz" },
    // LANG_STAB_PUMP
    { "Pomp", "Pump", "Pumpe" },
    // LANG_STAB_HEATING
    { "Verwarmen", "Heating", "Heizen" },
    // LANG_STAB_COOLING
    { "Koelen", "Cooling", "Kühlen" },
    // LANG_STAB_DHW
    { "Tapwater", "Hot water", "Warmwasser" },
    // LANG_STAB_BIJVERWARMEN
    { "Bijverwarmen", "Supplemental heat", "Zusatzheizung" },
    // LANG_STAB_SMARTGRID
    { "SmartGrid", "SmartGrid", "SmartGrid" },
    // LANG_STAB_WIFI
    { "WiFi", "WiFi", "WiFi" },

    // LANG_SET_THERMOSTAT_SOURCE
    { "Thermostaat bron", "Thermostat source", "Thermostatquelle" },
    // LANG_SET_THERMOSTAT_SOURCE_DESC
    { "Kies intern of extern thermostaatsignaal",
      "Choose internal or external thermostat signal",
      "Internen oder externen Thermostatsignal wählen" },
    // LANG_SET_DHW_PRESENT
    { "Tapwater aanwezig", "Hot water tank present", "Warmwasserspeicher vorhanden" },
    // LANG_SET_DHW_PRESENT_DESC
    { "Tapwatervat is aanwezig in het systeem",
      "Hot water tank is present in the system",
      "Warmwasserspeicher ist im System vorhanden" },
    // LANG_SET_ANALYTICS
    { "Analytics versturen", "Send analytics", "Analysen senden" },
    // LANG_SET_ANALYTICS_DESC
    { "Deel anonieme gebruiksdata voor het OpenAmber dashboard",
      "Share anonymous usage data for the OpenAmber dashboard",
      "Anonyme Nutzungsdaten für das OpenAmber-Dashboard teilen" },
    // LANG_SET_ANALYTICS_ID
    { "Analytics ID", "Analytics ID", "Analytics ID" },
    // LANG_SET_ANALYTICS_UNKNOWN
    { "Onbekend", "Unknown", "Unbekannt" },
    // LANG_SET_FROST1_TITLE
    { "Vorstbescherming buitentemperatuur 1e trap",
      "Frost protection outdoor temp stage 1",
      "Frostschutz Außentemperatur Stufe 1" },
    // LANG_SET_FROST1_DESC
    { "Buitentemperatuur waaronder trap 1 activeert",
      "Outdoor temperature below which stage 1 activates",
      "Außentemperatur, unterhalb derer Stufe 1 aktiviert" },
    // LANG_SET_FROST2_TA_TITLE
    { "Vorstbescherming buitentemperatuur 2e trap",
      "Frost protection outdoor temp stage 2",
      "Frostschutz Außentemperatur Stufe 2" },
    // LANG_SET_FROST2_TA_DESC
    { "Buitentemperatuur waaronder trap 2 activeert",
      "Outdoor temperature below which stage 2 activates",
      "Außentemperatur, unterhalb derer Stufe 2 aktiviert" },
    // LANG_SET_FROST2_TUI_TITLE
    { "Vorstbescherming watertemperatuur 2e trap",
      "Frost protection water temp stage 2",
      "Frostschutz Wassertemperatur Stufe 2" },
    // LANG_SET_FROST2_TUI_DESC
    { "Watertemperatuur waaronder trap 2 activeert",
      "Water temperature below which stage 2 activates",
      "Wassertemperatur, unterhalb derer Stufe 2 aktiviert" },
    // LANG_SET_PUMP_P1
    { "Pomp P1 aanwezig", "Pump P1 present", "Pumpe P1 vorhanden" },
    // LANG_SET_PUMP_P1_DESC
    { "Extra circulatiepomp P1 aanwezig",
      "Additional circulation pump P1 present",
      "Zusätzliche Umwälzpumpe P1 vorhanden" },
    // LANG_SET_PUMP_HEAT
    { "Pompsnelheid verwarmen", "Pump speed heating", "Pumpengeschwindigkeit Heizen" },
    // LANG_SET_PUMP_HEAT_DESC
    { "Pompsnelheid tijdens verwarmen",
      "Pump speed during heating",
      "Pumpengeschwindigkeit beim Heizen" },
    // LANG_SET_PUMP_DHW
    { "Pompsnelheid tapwater", "Pump speed hot water", "Pumpengeschwindigkeit Warmwasser" },
    // LANG_SET_PUMP_DHW_DESC
    { "Pompsnelheid tijdens het verwarmen van tapwater",
      "Pump speed during hot water heating",
      "Pumpengeschwindigkeit beim Warmwassererwärmen" },
    // LANG_SET_PUMP_INTERVAL
    { "Pompinterval (min)", "Pump interval (min)", "Pumpintervall (min)" },
    // LANG_SET_PUMP_INTERVAL_DESC
    { "Tussentijd periodiek pompen bij warmte-/koudevraag",
      "Interval for periodic pumping during heat/cooling demand",
      "Intervall für periodisches Pumpen bei Wärme-/Kältebedarf" },

    // LANG_SVC_CHECKING_UPDATE
    { "Controleren op update...", "Checking for update...", "Update wird geprüft..." },
    // LANG_SVC_UPDATE_BTN
    { "Update", "Update", "Aktualisieren" },
    // LANG_SVC_CHECK_UPDATES_BTN
    { "Controleer updates", "Check for updates", "Updates prüfen" },
    // LANG_SVC_FETCHING
    { "Ophalen...", "Fetching...", "Wird abgerufen..." },
    // LANG_SVC_QR_HINT
    { "Scan QR voor release info", "Scan QR for release info", "QR für Release-Info scannen" },
    // LANG_SVC_CONFIRM_TITLE
    { "Update bevestigen", "Confirm update", "Update bestätigen" },
    // LANG_SVC_CONFIRM_DESC
    { "Wil je de firmware update nu starten?",
      "Do you want to start the firmware update now?",
      "Möchtest du das Firmware-Update jetzt starten?" },
    // LANG_SVC_CANCEL
    { "Annuleren", "Cancel", "Abbrechen" },
    // LANG_SVC_START_UPDATE
    { "Update starten", "Start update", "Update starten" },
    // LANG_SVC_INSTALLING
    { "Update wordt geinstalleerd", "Installing update", "Update wird installiert" },

    // LANG_UPDATE_AVAILABLE_PREFIX
    { "Beschikbaar: ", "Available: ", "Verfügbar: " },
    // LANG_UPDATE_CURRENT_PREFIX
    { " (huidig: ", " (current: ", " (aktuell: " },
    // LANG_UPDATE_NONE
    { "Geen nieuwe software beschikbaar\nHuidige versie: ",
      "No new software available\nCurrent version: ",
      "Keine neue Software verfügbar\nAktuelle Version: " },
    // LANG_UPDATE_UNKNOWN
    { "onbekend", "unknown", "unbekannt" },
    // LANG_UPDATE_NO_SERVER
    { "Geen verbinding mogelijk met update server",
      "Unable to connect to update server",
      "Keine Verbindung zum Update-Server möglich" },
  };

  if (lang < 0 || lang > 2) lang = 0;
  return strings[key][lang];
}
