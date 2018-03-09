CC:= g++ -std=c++11

bin/main: build/User.o build/Date.o build/Meeting.o \
build/Storage.o build/AgendaService.o build/AgendaUI.o build/Agenda.o
	$(CC) -I./include $^ -o $@ -g
build/User.o: src/User.cpp include/User.hpp
	$(CC) -I./include -c src/User.cpp -o $@
build/Date.o: src/Date.cpp include/Date.hpp
	$(CC) -I./include -c src/Date.cpp -o $@
build/Meeting.o: src/Meeting.cpp include/Meeting.hpp
	$(CC) -I./include -c src/Meeting.cpp -o $@
build/Storage.o: include/Storage.hpp src/Storage.cpp
	$(CC) -I./include -c src/Storage.cpp -o $@
build/AgendaService.o: include/AgendaService.hpp src/AgendaService.cpp
	$(CC) -I./include -c src/AgendaService.cpp -o $@
build/AgendaUI.o: include/AgendaUI.hpp src/AgendaUI.cpp
	$(CC) -I./include -c src/AgendaUI.cpp -o $@
build/Agenda.o: src/Agenda.cpp
	$(CC) -I./include -c $^ -o $@
clean:
	@rm -rf build/*.o bin/*
