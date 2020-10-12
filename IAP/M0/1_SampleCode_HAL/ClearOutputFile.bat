::del *.uvmpw       /s  ::KEIL Multi-Project
del  *.uvgui.*      /s
del  *.wsdt         /s

::EWARM
del *.dep           /s
::del *.ewd         /s  ::Cannot delete IAR configuration file
::del *.ewp         /s  ::Cannot delete IAR project file
del *.ewt           /s
::del *.eww         /s  ::Cannot delete IAR IDE Workspace file

::EWARM\Debug\Exe
del *.out           /s
del *.bin           /s

::EWARM\Debug\List
del *.map           /s

::EWARM\Debug\Obj
del *.pbd.browse    /s
del *.pbi.dep       /s
del build.ninja     /s
del .ninja_deps     /s
del .ninja_log      /s
del *.o             /s
del *.pbd           /s
del *.pbi           /s
del *.pbw           /s
del *.xcl           /s

::EWARM\settings
del *.crun          /s
del *.dbgdt         /s
del *.cspy.bat      /s
del *.cspy.ps1      /s
del *.driver.xcl    /s
del *.general.xcl   /s
del *.dnx           /s
del *.wsdt          /s


::MDK-ARM
del *.uvguix.*      /s
::del *uvoptx       /s  ::Cannot delete KEIL configuration file
::del *uvprojx      /s  ::Cannot delete KEIL project file

::MDK-ARM\Listings
del *.map           /s
del *.lst           /s

::MDK-ARM\Objects
del *.axf           /s
del *.crf           /s
del *.d             /s
del *.dep           /s
del *.hex           /s
del *.lnp           /s
del *.o             /s
del *.build_log.htm /s
del *.htm           /s
del *.sct           /s
del *.bin           /s

::Source Insight
del *.IAB           /s
del *.IAD           /s
del *.IMB           /s
del *.IMD           /s
del *.PFI           /s
del *.PO            /s
del *.PR            /s
del *.PRI           /s
del *.PS            /s
del *.WK3           /s

exit
