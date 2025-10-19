1.  ESD is a Computer-Aided Design (CAD) software for the Windows environment, primarily used for Schematic design of electronic products and automated design (EDA) of Printed Circuit Boards (PCB).
2.  The ESD component library provides over 10,000 circuit component and package libraries from various manufacturers and for various functions.
3.  ESD is programmed with over 400,000 lines of C++ code, which is fully and completely open source.
4.  Using any version of Visual Studio 2010 or later, the final executable can be generated after compilation and linking.
5.  The current version of ESD is 4.0.
6.  ESD's native development and usage environment is Chinese. To facilitate communication, the main menu has been modified to be bilingual (Chinese/English); language prompts in other parts have not been changed yet.
7.  ESD's user interface programming uses Codejock's ToolkitPro. Before compiling the ESD code, you need to install the trial or commercial version of ToolkitPro v15 or later.
8.  The necessary personal setup involves using the Visual Studio project properties page to correctly set the following additional directories based on the installed ToolkitPro path (modify `"C:\Program Files (x86)"` as needed):
    *   **C/C++ -> Additional Include Directories:** `C:\Program Files (x86)\Codejock Software\MFC\Xtreme ToolkitPro v15.0.1\Source;%(AdditionalIncludeDirectories)`
    *   **Linker -> Additional Library Directories:** `C:\Program Files (x86)\Codejock Software\MFC\Xtreme ToolkitPro v15.0.1\lib\vc100;%(AdditionalLibraryDirectories)`
    *   **Resources -> Additional Include Directories:** `C:\Program Files (x86)\Codejock Software\MFC\Xtreme ToolkitPro v15.0.1\Source;%(AdditionalIncludeDirectories)`
9.  If you encounter any problems, please contact [...] for assistance.
10. Refer to the `readme.txt` files of other projects on GitHub for adding any necessary content.