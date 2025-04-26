import pygame
import sys
from settings import *
import text

_set=settings()
text_menu=["НАЧАЛО ИГРЫ","ИНСТРУКЦИЯ",\
"ВЫХОД","Команда 1","Команда 2"]
#текст на кнопках

#передача классу MENU поверхности
#массивов кнопок меню и шрифта
class MENU():
	def __init__(self,sc,menu_butt,inputbox):
		self.sc=sc
		self.menu_butt=menu_butt
		self.inputbox=inputbox
		self.nameteam=['','']
#метод run отрисовывает меню
	def run(self,info):	
		game=True #игра активна
		clock=pygame.time.Clock()
		while game:
			_set.menu_num=-1  #если курсор не на кнопке, то значение -1
			self.sc.blit(_set.bg_menu,(0,0)) #отображение заднего фона меню
			self.sc.blit(_set.font[3].render(\
			_set.caption,1,(0,0,0)),(33,30)) #отрисовка названия игры
			self.sc.blit(_set.font[3].render(\
			_set.caption,1,(155,155,155)),(30,30))
				
			
			#отрисовка 3 кнопок
			for i in range(len(self.menu_butt)):
				self.menu_butt[i].draw(\
				self.menu_butt[i].imgInact) #отрисовка неактивных кнопок
				if self.menu_butt[i].check_pos()==1:
					#если мышь наведена на кнопку - она становится активной и запоминается её номер
					self.menu_butt[i].draw(\
					self.menu_butt[i].imgAct) 
					_set.menu_num=i
			
			#отрисовка надписей на кнопках		
			for i in range(3):
				self.sc.blit(_set.font[5].\
				render(text_menu[i],\
				1,(0,0,0)),(47,(i*60)+405))
	
			self.sc.blit(_set.font[5].\
			render(text_menu[3],\
			1,(155,155,155)),(387,205)) #надпись "команда 1"
			self.sc.blit(_set.font[5].\
			render(text_menu[4],\
			1,(155,155,155)),(587,205)) #надпись "команда 2"
			self.sc.blit(_set.font[5].\
			render(text_menu[3],\
			1,(200, 0, 0)),(385,205))  #надпись "команда 1" с другим цветом
			self.sc.blit(_set.font[5].\
			render(text_menu[4],\
			1,(200, 0, 0)),(585,205))  #надпись "команда 2" с другим цветом
			
			for event in pygame.event.get():
				#проверка на выход из приложения
				if event.type==pygame.QUIT:
					sys.exit()
				#проверка нажатия на одну из 3 кнопок
				elif event.type==pygame.MOUSEBUTTONDOWN:
					if event.button==1:
						#если нажата 1 кнопка, то проверяется заполненность названий команд
						#если названия команд пусты, то переменная _set.names=False
						#иначе начинается игра
						if _set.menu_num==0:
							if self.nameteam\
							[0]=='' \
							or self.nameteam\
							[1]=='':
								_set.names\
								=False
							else:
								_set.names\
								=True
								game=False
						#если нажата 2 кнопка-инструкция
						elif _set.menu_num==1:
							info.run()
						#если нажата 3 кнопка-выход из программы
						elif _set.menu_num==2:
							sys.exit()
						
							
				for i in range(2):
					self.inputbox[i+1].\
					check_event(event) #проверка на ввод названий в поля ввода
					self.nameteam[i]=\
					self.inputbox[i+1].text #запись имён команд в главном меню
			
			for i in range(1,3):
				self.inputbox[i].draw()	 #отрисовка полей ввода			
			
			#если названия команд пусты, то высвечивается соответствующая надпись;
			if _set.names==False:
				_set.delay+=1
				if _set.delay<=90:
					#высвечивание на экране сообщения об ошибке с именами
					self.sc.blit(_set.nameteams,\
					(460,300)) 
				else:
					_set.delay=0
					_set.names=None



			pygame.display.update()
			clock.tick(_set.fps)


class INFO():
	def __init__(self,sc,sys_butt):
		self.sc=sc
		self.sys_butt=sys_butt
		self.CanClick=False

	def run(self):
		game=True
		clock=pygame.time.Clock()
		while game:
			self.CanClick=False  #переменная, показывающая возможность нажать на кнопку
			self.sc.blit(_set.bg_menu,(0,0))  #отображение заднего фона инструкции
			
			text.draw_info(self.sc)  #отрисовка текста инструкции
			
			#отрисовка кнопки внизу для выхода в меню
			self.sys_butt[3].draw(\
			self.sys_butt[3].imgInact)
			if self.sys_butt[3].check_pos()==1:
				self.sys_butt[3].draw(\
				self.sys_butt[3].imgAct)
				self.CanClick=True  #если наведён курсор на кнопку, то переменная CanClick=True
				

			#отрисовка надписи "назад в меню" на кнопке внизу
			self.sc.blit(_set.font[4].render(\
			'НАЗАД В МЕНЮ',1,(0,0,0)),(350,556))
			
			
			for event in pygame.event.get():
				#проверка на выход из приложения
				if event.type==pygame.QUIT:
					sys.exit()
				elif event.type==\
				pygame.MOUSEBUTTONDOWN: 
					if event.button==1:
						#если нажата кнопка внизу-происходит выход назад в меню
						if self.CanClick==True:
								game=False




			pygame.display.update()
			clock.tick(_set.fps)
