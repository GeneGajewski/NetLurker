# ![NetLurker Dude](anonymity_100x.png) NetLurker
---
*Lightweight* monitoring application for [NetLogger](https://www.netlogger.org) amateur radio nets.

This application is built using Embarcadero [C++ Builder Community Edition](https://www.embarcadero.com/products/cbuilder/starter/),
which is free to use for non-commercial purposes. I chose this as I am a longtime user of C++ Builder and I appreciate the simplicity
of the C++ Builder/Delphi runtime library ecosystem for building native desktop apps.
Besides, I've been around since the [Turbo C 1.5](https://en.wikipedia.org/wiki/Borland_Turbo_C) days.

This is a straightforward project built using the IDE so there is no complicated build environment to deal with.
It uses the FireMonkey UI library, so there is potential for crossplatform builds for Mac, iOS, and Android in the future, if desired.
It should be possible to publish via the Windows Store as well.
Installs are built using the free-to-use version of [Advanced Installer](https://www.advancedinstaller.com/).
There is plenty of potential for adding new features.
The goal here is to be exceedingly simple, self explanatory and easy on the eyes - less distraction, more ***radio***!

Program updates can be queried for within the program and can be installed if desired.
I created a modified concept of the simple [WinGup](https://wingup.org/) system used by [Notepad++](https://notepad-plus-plus.org/) (an awesome program editor!) to handle program update duties. Simply, the application downloads the latest installer version,
(an .MSI file) then calls ShellExcute to run the install and then closes itself (the running application) so the the install can complete successfully.


[WG5ENE](https://qrz.com/db/wg5ene)


