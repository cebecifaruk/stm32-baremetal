@echo off
title Expkits - Proje Temizlik Uygulaması
ECHO Gereksiz proje dosyaları temizleniyor...
:Output
If Exist Output\ (goto CleanOutput)
:Obj
If Exist Obj\ (goto CleanObj)
:List
If Exist List\ (goto CleanList) else goto CleanHere
:CleanOutput
  DEL *.HEX
  CD Output
  copy *.hex ..\
  DEL *.* /S /Q >..\log.txt
  CD..
  goto Obj

:CleanObj
  CD Obj
  DEL *.* /S /Q >..\log.txt
  CD..
  RD Obj
  goto List

:CleanList
  CD List
  DEL *.* /S /Q >..\log.txt
  CD..
  RD List
:CleanHere
DEL *.obj /Q
DEL *.__i /Q
DEL *.axf /Q
DEL *.lst /Q
DEL *.map /Q
DEL *.o /Q
DEL *.d /Q
DEL *.crf /Q
DEL *.tra /Q
DEL *.sct /Q
DEL *.dep /Q >\log.txt
DEL Jlink*.* /Q >\log.txt
DEL JLinkLog.txt >\log.txt
DEL *.bak /Q>\log.txt
DEL *uvgui.%USERNAME% /Q>\log.txt
DEL *uvgui.* /Q>\log.txt
ECHO Tamamlandı..
ECHO Sonuçları görüntülemek isterseniz log.txt dosyasına bakabilirsiniz.
PAUSE
DEL log.txt