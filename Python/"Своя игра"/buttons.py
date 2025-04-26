import pygame
from settings import settings

_set=settings()

#класс кнопок
class button():
	def __init__(self,sc,x,y,imgInact\
	,imgAct,buttW,buttH,score):
		self.sc=sc
		self.x=x
		self.y=y
		self.imgInact=imgInact
		self.imgAct=imgAct
		self.width=buttW
		self.height=buttH
		self.score=score
	
#рисование кнопки на экране	
	def draw(self,image):
		self.sc.blit(image,\
		(self.x,self.y))

#проверка пересечения курсора с кнопкой
	def check_pos(self):
		mp=pygame.mouse.get_pos()
		if (self.x<=mp[0]<=self.x+self.width) \
		and (self.y<=mp[1]<=self.y+self.height):
			return 1
		else:
			return 0
