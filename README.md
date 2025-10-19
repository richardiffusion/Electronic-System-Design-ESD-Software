# ESD V4 - Open Source Electronic Design Automation (EDA) Software

A comprehensive Windows-based CAD/EDA tool for schematic capture (SCH) and printed circuit board (PCB) design. Features extensive component libraries, auto-routing, DRC, and support for multi-sheet designs. Originally developed in Chinese, this open-source version provides full access to the C++ source code for educational and research purposes.

- 🔧 Built with C++ and MFC using Codejock ToolkitPro
- 📚 Over 10,000 components in library
- ⚡ Real-time DRC and auto-routing
- 🎯 Professional schematic & PCB layout tools

## System Requirements
- Windows 7/10/11
- Visual Studio 2010 or later
- Codejock ToolkitPro v15.0.1+

## Quick Start

### Prerequisites
1. Install Visual Studio 2010+
2. Install Codejock ToolkitPro v15.0.1+
3. Clone this repository

### ⚠️ Important Notes
- **Windows Only**: Built for Windows environments
- **Educational Use**: For learning and research purposes
- **Third-party Dependencies**: Commercial use requires ToolkitPro license

### 🔧 System Requirements
- Windows 7/10/11
- .NET Framework 4.5+
- 2GB RAM minimum, 4GB recommended

### 📚 Documentation
See repository README.md and README.txt for build instructions and source code details.

---
*For educational and research purposes. Commercial use requires proper licensing of third-party dependencies.*

# Quick Overview of EDS V4 Software Main Functions & Basic Usage

## The schematic design environment is abbreviated as SCH, and the printed circuit board design environment is abbreviated as CPB.

## First, run ESD.exe. The SCH interface will appear. Execute the **File/Open** command and open the sample design file `controller.eda` located in the `demo` folder.

<img width="1381" height="1071" alt="SCH-1" src="https://github.com/user-attachments/assets/74fb82cf-5baa-4f55-b8c8-dc32b9f2044e" />

## Use the **View/All** command or **View/Sheet** command to display the full view of the design (Shortcuts are **Z-A** and **Z-S**).

## Use the **View/In** command or **View/Out** command to zoom in or out of the design view (Shortcuts are **Z-I** or **PgUp**, and **Z-O** or **PgDn**).

## The mouse wheel, **Shift + mouse wheel**, holding the right mouse button and dragging, **PgUp**, and **PgDn** are all quick ways to control the display area.

<img width="1377" height="1060" alt="SCH-2" src="https://github.com/user-attachments/assets/4cf91609-6063-426e-a7a0-f3b2e356f51c" />

## Press the **Tab** key, use the **View/To PCB** command, or click the toolbar icon [https://SCHTool-1.png](https://schtool-1.png/) to switch to the PCB design environment.

<img width="1381" height="1068" alt="PCB-1" src="https://github.com/user-attachments/assets/5ae1f472-7fe3-4fc4-8056-2c0c8336ef9e" />

## The SCH and PCB environments work together to complete the process from schematic design to PCB realization. The component list and netlist from SCH can be automatically transferred to the PCB environment, enabling automated design processes such as loading footprints, component placement, routing, and Design Rule Checking (DRC). Of course, using only the SCH part or only the PCB part can also fulfill user requirements. For example, even with only a PCB file, it's possible to generate a component list and netlist from the PCB, and then proceed with DRC.

## Operations on objects in both SCH and PCB follow the general standards of CAD design software: **Left-click = Select, Double-click = Edit, Hold left button and drag = Move**. All editing commands for objects are located under the main menu "Edit", and all placement commands are under the main menu "Place". The software fully supports **Undo** and **Redo** for operations.

# The following images demonstrate various operations and functions in SCH and PCB

## SCH - Place Command Menu

<img width="1382" height="1067" alt="SCH-3" src="https://github.com/user-attachments/assets/f07158e4-8d8d-4811-8fcc-f5eb41ef5cc3" />

## PCB - Place Command Menu

<img width="1381" height="1066" alt="PCB-2" src="https://github.com/user-attachments/assets/9449b92c-7490-4ccc-bd0e-f2e3f4e907fa" />

## SCH - Editing a Component (Allows simultaneous definition of the component's Package)

<img width="1380" height="1066" alt="SCH-4" src="https://github.com/user-attachments/assets/9aed4938-4598-4b87-aede-242a44b30cb8" />

## SCH - Editing a Power Object

<img width="1381" height="1067" alt="SCH-5" src="https://github.com/user-attachments/assets/8d67b9ca-e875-45e7-942d-8fe16b4204bb" />

## SCH - Editing an Arc

<img width="1383" height="1066" alt="SCH-6" src="https://github.com/user-attachments/assets/d62abfe1-9702-4f82-8169-29f35887c512" />

## PCB - Multi-selection Window during Editing

<img width="1379" height="1067" alt="PCB-3" src="https://github.com/user-attachments/assets/211ca73c-b844-4e63-8fb8-f8987e1f4459" />

## PCB - Editing a Pad

<img width="1383" height="1069" alt="PCB-4" src="https://github.com/user-attachments/assets/ca0b2a79-45d8-40c2-9f9c-750e2e2d2798" />

## PCB - Copper Pour

<img width="1382" height="1068" alt="PCB-5" src="https://github.com/user-attachments/assets/02bf6226-0530-4353-bfea-021cfc09dc51" />

## PCB - Editing Text

<img width="1375" height="1063" alt="PCB-6" src="https://github.com/user-attachments/assets/89e46915-0417-4a66-a606-c081f81da52b" />

## Supports Print, Plot, and Gerber Output - Print Preview

<img width="1376" height="1064" alt="SCH-7" src="https://github.com/user-attachments/assets/f9bfc43a-2f6e-432a-b5b8-e9e8e911090a" />

## SCH Environment Supports Multi-sheet Designs - Sheet Display Control Tab

<img width="623" height="294" alt="SCH-8" src="https://github.com/user-attachments/assets/0dc7590f-f8a9-4f1d-834f-649a39af9969" />

## SCH Component Libraries and PCB Footprint Libraries contain tens of thousands of components and support user customization.

<img width="1379" height="1066" alt="SCH-9" src="https://github.com/user-attachments/assets/15088c57-202c-4639-a1a9-ce36eb8f37ad" />

<img width="1378" height="1065" alt="PCB-7" src="https://github.com/user-attachments/assets/7f50f397-1c98-4327-803c-1dde375cadd5" />

## SCH Highlight Net

<img width="1378" height="1066" alt="SCH-10" src="https://github.com/user-attachments/assets/1a534626-dfdb-4590-b871-a7ee9a36f795" />

## SCH Highlight Net Pin List

<img width="1375" height="1065" alt="SCH-11" src="https://github.com/user-attachments/assets/5aaea6da-4138-4d62-975c-526855533ec5" />

## PCB Highlight Net and Pin List

<img width="1376" height="1067" alt="PCB-8" src="https://github.com/user-attachments/assets/6653145c-90ef-4fa4-8abe-9608bba6cdeb" />

## SCH Compilation and Various Report Lists

<img width="1381" height="1068" alt="SCH-12" src="https://github.com/user-attachments/assets/ed85f0c9-754e-4f36-980f-26efe01c470b" />

## PCB Ratsnest Display, Supports Automatic Optimization

<img width="1379" height="1063" alt="PCB-9" src="https://github.com/user-attachments/assets/9b81b7d8-8b04-45e4-b6a6-dc902d263ec5" />

## PCB Manual Routing, Supports Real-time DRC, Auto-routing

<img width="1381" height="1064" alt="PCB-10" src="https://github.com/user-attachments/assets/5b02b789-dd6c-4e6f-af63-6f84b4ead6f0" />

## PCB Design Layer Planning and Control

<img width="1672" height="1273" alt="PCB-11" src="https://github.com/user-attachments/assets/1525ab91-f5e9-4a35-8031-07fd252c8c22" />

## PCB Design Rule Check (DRC)

<img width="1396" height="1056" alt="PCB-12" src="https://github.com/user-attachments/assets/08b0b426-9462-458d-b115-8373d1988e77" />

# The complete software functionality and user manual cover over 300 pages, which cannot be detailed here. Interested users or developers can contact the author for further assistance.
