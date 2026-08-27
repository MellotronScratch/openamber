# Testdocument

xychart-beta
    title "Maximale aanvoertemperatuur (Tuo)"
    x-axis "Buiten (°C)" [-25, "-20", "-15", "-10", "-5", "0", "5", "10", "15", "20", "25", "30", "35", "40", "45"]
    y-axis "Max. aanvoer (°C)" 40 --> 75
    line [50, 52, 55, 70, 70, 70, 70, 70, 70, 70, 55, 52, 50, 50, 50]

flowchart LR
    subgraph Extreme_Koud["< -15°C"]
        direction TB
        A["50°C aanvoer"]
    end
    
    subgraph Transition_Koud["-15°C tot -10°C"]
        B["50 → 55 → 70°C"]
    end
    
    subgraph Optimum["-10°C tot 20°C"]
        C["70°C (maximaal)"]
    end
    
    subgraph Transition_Warm["20°C tot 25°C"]
        D["70 → 55°C"]
    end
    
    subgraph Extreme_Warm["> 25°C"]
        E["55 → 50°C"]
    end
    
    Extreme_Koud --> Transition_Koud --> Optimum --> Transition_Warm --> Extreme_Warm