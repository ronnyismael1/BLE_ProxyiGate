# Project Idea - RFID Blocked Litter Box

## References
Example projects for the ESP32:
PATH:`~/esp/esp-idf/examples`

Guide for ESP32 projects:
URL:[Introduction to ESP32 Development](https://ncona.com/2024/08/introduction-to-esp32-development/)
URL:[Neovim as ESP32 IDE with Cland LSP](https://ncona.com/2024/0/neovim-as-esp32-ide-with-clangd-lsp/)

## Project Structure
```

`-- project_mcu_dog
    |-- README.md
    |-- architecture
    |-- design
    |-- implementation
    |   `-- src
    |       `-- main.c
    |-- releases
    `-- validation
```
```
```

1. Architecture   - Simulations and schematics 
2. Design         - BOM and other misc
3. Implementation - Code source
4. Validation     - Test cases
5. Releases       - Build directory


## Rough Idea
* Simon will wear a RFID tag on his collar
* RFID reader scans entrance to litter box
* If Simon's tag is detected then the flap closes


## Limitations
1. RFID modules usually have short range
2. Dog might move fast back and forth, add lockout period?


## Seudo BOM
1. RC522 RFID module (ID-12LA-HE? 17cm distance)
2. Microcontroller (arduino, esp32, ...)
3. Some sort of servo
4. misc wires and stuff

