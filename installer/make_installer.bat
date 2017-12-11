@echo ===========================================
@echo          Making Installer Start
@echo ===========================================

SET target=iastpp_1.0.1_windows_installer.exe
del %target%

@echo               Generating...

@echo ===========================================
@echo       Copy binaries to data folder...
@echo ===========================================

xcopy ..\build_msvc\release packages\kaist.molsim.iastpp\data\ /e /h /k /Y

@echo ===========================================
@echo              Clean up folder...
@echo ===========================================

cd packages\kaist.molsim.iastpp\data
del *.obj *.cpp *.res *.h
cd ..\..\..

@echo ===========================================
@echo       Copy examples to data folder...
@echo ===========================================

xcopy ..\examples packages\kaist.molsim.iastpp\data\examples\ /e /h /k /Y

@echo ===========================================
@echo              Make installer...
@echo ===========================================

binarycreator.exe -c config\config.xml -p packages %target%

@echo ===========================================
@echo                Job Done.
@pause
