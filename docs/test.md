# Testdocument

xychart-beta
    title "Maximale aanvoertemperatuur (Tuo))"
    x-axis "Buitentemperatuur (°C)" [-25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45]
    y-axis "Maximale aanvoertemperatuur (°C)" 40 --> 75
    line [50, 52, 55, 70, 70, 70, 70, 70, 70, 70, 55, 52, 50, 50, 50]

```mermaid
xychart-beta
    title "Maximale aanvoertemperatuur (Tuo)"
    x-axis "Buitentemperatuur (°C)" [-25, -20, -15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45]
    y-axis "Maximale aanvoertemperatuur (°C)" 40 --> 75
    line [50, 52, 55, 70, 70, 70, 70, 70, 70, 70, 55, 52, 50, 50, 50]


```markdown
| Buiten (°C) | Max. aanvoer (°C) |
|:-----------:|:-----------------:|
| -25         | 50                |
| -15         | 55                |
| -10         | **70**            |
| 0           | **70**            |
| 20          | **70**            |
| 25          | 55                |
| 35          | 50                |
| 45          | 50                |


gantt
    title Maximale aanvoertemperatuur (Tuo)
    dateFormat X
    axisFormat %s
    
    section Temp zone
    50°C (extreme zones) :done, 0, 2
    55°C (transition) :active, 2, 3
    70°C (optimum) :crit, 3, 9
    55°C (transition) :active, 9, 10
    50°C (extreme zones) :done, 10, 14
    
    section Buitentemp
    -25 tot -15 : 0, 2
    -15 tot -10 : 2, 3
    -10 tot 20 : 3, 9
    20 tot 25 : 9, 10
    25 tot 45 : 10, 14