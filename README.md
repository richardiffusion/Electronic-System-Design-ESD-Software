# Quick Overview of EDS V4 Software Main Functions & Basic Usage

## The schematic design environment is abbreviated as SCH, and the printed circuit board design environment is abbreviated as CPB.

## First, run ESD.exe. The SCH interface will appear. Execute the **File/Open** command and open the sample design file `controller.eda` located in the `demo` folder.

[https://SCH-1.png](https://sch-1.png/)

## Use the **View/All** command or **View/Sheet** command to display the full view of the design (Shortcuts are **Z-A** and **Z-S**).

## Use the **View/In** command or **View/Out** command to zoom in or out of the design view (Shortcuts are **Z-I** or **PgUp**, and **Z-O** or **PgDn**).

## The mouse wheel, **Shift + mouse wheel**, holding the right mouse button and dragging, **PgUp**, and **PgDn** are all quick ways to control the display area.

[https://SCH-2.png](https://sch-2.png/)

## Press the **Tab** key, use the **View/To PCB** command, or click the toolbar icon [https://SCHTool-1.png](https://schtool-1.png/) to switch to the PCB design environment.

[https://PCB-1.png](https://pcb-1.png/)

## The SCH and PCB environments work together to complete the process from schematic design to PCB realization. The component list and netlist from SCH can be automatically transferred to the PCB environment, enabling automated design processes such as loading footprints, component placement, routing, and Design Rule Checking (DRC). Of course, using only the SCH part or only the PCB part can also fulfill user requirements. For example, even with only a PCB file, it's possible to generate a component list and netlist from the PCB, and then proceed with DRC.

## Operations on objects in both SCH and PCB follow the general standards of CAD design software: **Left-click = Select, Double-click = Edit, Hold left button and drag = Move**. All editing commands for objects are located under the main menu "Edit", and all placement commands are under the main menu "Place". The software fully supports **Undo** and **Redo** for operations.

# The following images demonstrate various operations and functions in SCH and PCB

## SCH - Place Command Menu

[https://SCH-3.png](https://sch-3.png/)

## PCB - Place Command Menu

[https://PCB-2.png](https://pcb-2.png/)

## SCH - Editing a Component (Allows simultaneous definition of the component's Package)

[https://SCH-4.png](https://sch-4.png/)

## SCH - Editing a Power Object

[https://SCH-5.png](https://sch-5.png/)

## SCH - Editing an Arc

[https://SCH-6.png](https://sch-6.png/)

## PCB - Multi-selection Window during Editing

[https://PCB-3.png](https://pcb-3.png/)

## PCB - Editing a Pad

[https://PCB-4.png](https://pcb-4.png/)

## PCB - Copper Pour

[https://PCB-5.png](https://pcb-5.png/)

## PCB - Editing Text

[https://PCB-6.png](https://pcb-6.png/)

## Supports Print, Plot, and Gerber Output - Print Preview

[https://SCH-7.png](https://sch-7.png/)

## SCH Environment Supports Multi-sheet Designs - Sheet Display Control Tab

[https://SCH-8.png](https://sch-8.png/)

## SCH Component Libraries and PCB Footprint Libraries contain tens of thousands of components and support user customization.

[https://SCH-9.png](https://sch-9.png/)  
[https://PCB-7.png](https://pcb-7.png/)

## SCH Highlight Net

[https://SCH-10.png](https://sch-10.png/)

## SCH Highlight Net Pin List

[https://SCH-11.png](https://sch-11.png/)

## PCB Highlight Net and Pin List

[https://PCB-8.png](https://pcb-8.png/)

## SCH Compilation and Various Report Lists

[https://SCH-12.png](https://sch-12.png/)

## PCB Ratsnest Display, Supports Automatic Optimization

[https://PCB-9.png](https://pcb-9.png/)

## PCB Manual Routing, Supports Real-time DRC, Auto-routing

[https://PCB-10.png](https://pcb-10.png/)

## PCB Design Layer Planning and Control

[https://PCB-11.png](https://pcb-11.png/)

## PCB Design Rule Check (DRC)

[https://PCB-12.png](https://pcb-12.png/)

# The complete software functionality and user manual cover over 300 pages, which cannot be detailed here. Interested users or developers can contact the author for further assistance.