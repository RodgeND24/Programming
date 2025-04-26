import pygame, os
from buttons import *
from settings import *
from screen_draw import *
from levels import *
from inputAnswer import *

def all_buttons(sc,_set,buttons,sys_butt,menu_butt): #функция объявления всех массивов кнопок
	
	#очистка всех массивов
	buttons.clear()
	sys_butt.clear()
	menu_butt.clear()
	
	#задание 2-мерного массива с объектами "кнопка с баллами"
	for j in range(5):
                for i in range(5):
                        buttons.append(button\
                        (sc,(i*110)+32,(j*100)+40,\
                        _set.image_buttons[i],\
                        _set.click_buttons[i],\
                        _set.butt1W,_set.butt1H,\
                        (i+1)*100))
	
	#задание массива с кнопками Меню
	for i in range(3):
		menu_butt.append(button(sc,30,(i*60)+400,\
		_set.menu_butt1,_set.menu_butt2,\
		_set.butt3W,_set.butt3H,None))

	sys_butt.append(button(sc,415,420,_set.im_sys_buttons[0],\
	_set.im_sys_buttons[1],_set.butt2W,_set.butt2H,None)) #кнопка "пропустить"  #0#
	sys_butt.append(button(sc,255,420,_set.im_sys_buttons[0],\
	_set.im_sys_buttons[1],_set.butt2W,_set.butt2H,None)) #кнопка "проверить" #1#
	sys_butt.append(button(sc,335,400,_set.im_sys_buttons[0],\
	_set.im_sys_buttons[1],_set.butt2W,_set.butt2H,None)) #кнопка "меню" после конца игры #2#
	sys_butt.append(button(sc,335,550,_set.im_sys_buttons[0],\
	_set.im_sys_buttons[1],_set.butt2W,_set.butt2H,None)) #кнопка "меню" для выхода из инструкции #3#
	sys_butt.append(button(sc,660,565,_set.im_sys_buttons[0],\
	_set.im_sys_buttons[1],_set.butt2W,_set.butt2H,None)) #кнопка "Выход" во время игры #4#
	sys_butt.append(button(sc,225,420,_set.im_sys_buttons[0],\
	_set.im_sys_buttons[1],_set.butt2W,_set.butt2H,None)) #кнопка "Уверен" #5#
	sys_butt.append(button(sc,425,420,_set.im_sys_buttons[0],\
	_set.im_sys_buttons[1],_set.butt2W,_set.butt2H,None)) #кнопка "Не Уверен" #6#


def main():
	os.environ['SDL_VIDEO_CENTERED']='1' #центровка окна в середине экрана


	pygame.init()
	_set=settings() #класс глобальных переменных,
					#из него берутся значения большинства переменных
					
	sc=pygame.display.set_mode((_set.WINwidth,_set.WINheight)) #инициализация поверхности
	#WINwidth, WINheight - ширина и высота окна
	pygame.display.set_caption(_set.caption) #название приложения
	pygame.display.set_icon(_set.icon) #иконка игры
	clock=pygame.time.Clock() #класс для управление временем


	buttons=[]  #массив кнопок с баллами
	sys_butt=[] #массив с системными кнопками
	menu_butt=[]  #массив с кнопками Меню

		
	all_buttons(sc,_set,buttons,sys_butt,menu_butt)#объявление всех кнопок

	inputbox=[InputBox(sc,230,380,340,30,23), #поле ввода ответа
	InputBox(sc,360,240,173,30,11), #поле ввода имени первой команды
	InputBox(sc,560,240,173,30,11)] #поле ввода имени второй команды

	menu=MENU(sc,menu_butt,inputbox) #создание класса МЕНЮ
	info=INFO(sc,sys_butt) #создание класса ИНСТРУКЦИЯ

	menu.run(info) #запуск игры методом run меню игры
	while 1:
		game=True
		
		while game:
			#keys = pygame.key.get_pressed() #если в игре нажать escape, то произойдёт выход в меню
			#if keys[pygame.K_ESCAPE]:
			#	game=False
				
			#передача в функцию screen_draw массивов всех кнопок, окна ввода, названий команд
			#функция возвращает True или False, если окончена или не окончена игра соответственно	
			_set.end_game=screen_draw(sc,buttons,\
			sys_butt,inputbox,menu.nameteam)

			pygame.display.update() #обновление экрана
			clock.tick(_set.fps) #установка частота кадров
			
			#если конец игры, что цикл прерывается, массивы кнопок очищаются и заново переприсваиваются
			if _set.end_game==True:
				_set.butt_count=25  #обновляется количество кнопок
				all_buttons(sc,_set,buttons,sys_butt,menu_butt) #обновление всех кнопок
				
				menu.nameteam=['','']  #обнуление названий команд
				menu.inputbox[1].update() #очищение поля ввода в меню
				menu.inputbox[2].update() #очищение поля ввода в меню
				inputbox[1].k=inputbox[2].k=0  #длина слова снова может увеличиваться
				
				game=False  #выход в главное меню после подведения итогов
				
		menu.run(info) #старт меню после конца предыдущей игры

if __name__=="__main__":
	main()
