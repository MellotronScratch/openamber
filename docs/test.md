```mermaid
%%{
  init: {
    'xyChart': {
      'width': 500,
      'height': 300,
      'titleFontSize': 16,
      'showDataLabel': true,
      'xAxis': {
        'labelFontSize': 14,
        'labelRotation': 45,
        'titleFontSize': 12,
        'titlePadding': 10,
        'tickLength': 8,
        'axisLineWidth': 3
      },
      'yAxis': {
        'labelFontSize': 14,
        'titleFontSize': 12,
        'showTick': true,
        'tickLength': 5
      }
    }
  }
}%%
xychart-beta
    title "Maximale aanvoertemperatuur (Tuo)"
    x-axis "Buitentemperatuur (°C)" [-25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45]
    y-axis "Max. aanvoer (°C)" 40 --> 75
    line [50, 52, 55, 70, 70, 70, 70, 70, 70, 70, 55, 52, 50, 50, 50]
