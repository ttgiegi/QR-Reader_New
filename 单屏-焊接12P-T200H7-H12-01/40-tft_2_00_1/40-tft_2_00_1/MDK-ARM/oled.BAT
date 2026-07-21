SET PATH=D:\software\keil5\ARM\ARMCC\Bin;D:\software\python\Scripts\;D:\software\python\;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\windows\System32\OpenSSH\;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Users\Administrator.SY-202502241638\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=oled
SET CPU_CLOCK=0x00B71B00
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\tftlcd.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\core_cm3.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_it.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\system_stm32f10x.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_stm32f10x_md._ia"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\misc.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_gpio.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_rcc.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_rtc.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_spi.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\delay.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\sys.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"D:\software\keil5\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\oled.lnp"
"D:\software\keil5\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\oled.axf" --i32combined --output ".\Objects\oled.hex"
