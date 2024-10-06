all: build_echo build_pwd build_cd ./shell_code/shell.c
	gcc ./echo_code/echo.o ./pwd_code/pwd.o ./cd_code/cd.o ./shell_code/shell.c -o shell.exe -Wall

build_echo: ./echo_code/echo.c
	gcc -c ./echo_code/echo.c -o ./echo_code/echo.o -Wall

build_pwd: ./pwd_code/pwd.c
	gcc -c ./pwd_code/pwd.c -o ./pwd_code/pwd.o -Wall

build_cd: ./cd_code/cd.c
	gcc -c ./cd_code/cd.c -o ./cd_code/cd.o -Wall

clear: clear_shell clear_echo clear_pwd clear_cd

clear_echo:
	cd .\echo_code && \
	del *.o && \
	cd ..

clear_pwd:
	cd .\pwd_code && \
	del *.o && \
	cd ..

clear_cd:
	cd .\cd_code && \
	del *.o && \
	cd ..

clear_shell:
	del *.exe
