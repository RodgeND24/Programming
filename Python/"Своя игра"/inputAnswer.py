import pygame
from settings import *

pygame.init()

Color_active=(45, 162, 204) #цвет активного окна ввода
Color_inactive =(175, 213, 227) #цвет неактивного окна ввода
_set=settings()

class InputBox:

	def __init__(self,sc, x, y,\
	 width, height,strlen):
		self.sc=sc
		self.rect = pygame.Rect\
		(x, y, width, height) #прямоугольник с координатами, длиной и шириной
		self.color = Color_inactive  #цвет неактивного поля
		self.text = ''  #переменная для накопления введённого текста
		self.text_surf = _set.font[0]\
		.render(self.text, True,\
		(0,0,0))  #цвет текста названий команд
		self.active = False
		self.font=_set.font[0] #шрифт
		self.strlen=strlen  #максимальная длина строки
		self.k=0

	def check_event(self, event):
		if event.type == \
		pygame.MOUSEBUTTONDOWN:
			#если нажата кнопка мыши
			if self.rect.\
			collidepoint(event.pos):
				#кнопка становится активной
				self.active = True
			else:
				#иначе не активна
				self.active = False
		
		#если кнопка активная, то цвет окна ввода меняется на активный
		if self.active==True:
			self.color = Color_active
		else:
			self.color=Color_inactive
		
		#если окно активно и нажимаются клавиши
		if self.strlen==None:
			if event.type == \
			pygame.KEYDOWN:
				if self.active==True:
					#если нажата клавиша backspace, то последний символ во введённой строке удаляется
					if event.key ==\
					 pygame.K_BACKSPACE:
						self.text = \
						self.text[:-1]
					else:
					#иначе в строку записывается символ нажатой клавиши
							self.text += \
							str(event.unicode)
					#отрисовка введённого символа в окне ввода
					self.text_surf = \
					self.font.render\
					(self.text, True, (0,0,0))
					
		elif event.type == pygame.KEYDOWN:
			if self.active==True:
				#если нажата клавиша backspace, то последний символ во введённой строке удаляется
				if event.key == \
				pygame.K_BACKSPACE \
				and self.k!=0:
					self.text = \
					self.text[:-1]
					self.k-=1
				elif self.k<self.strlen:
					#иначе в строку записывается символ нажатой клавиши
					self.text += str(event.unicode)
					self.k+=1
				#отрисовка введённого символа в окне ввода
				self.text_surf = \
				self.font.render\
				(self.text, True, (0,0,0))
			
	
	#метод отрисовки поля ввода
	def draw(self):
		#отрисовка прямоугольника окна ввода
		pygame.draw.rect\
		(self.sc, (221, 210, 236), self.rect)
		pygame.draw.rect\
		(self.sc, self.color, self.rect,2)
		#отрисовка текста
		self.sc.blit(self.text_surf, \
		(self.rect.x+5, self.rect.y-1))
		
	
	#метод очистки полей ввода
	def update(self):
		self.text=''
		self.text_surf=self.font.render\
		('', True, (0,0,0))
