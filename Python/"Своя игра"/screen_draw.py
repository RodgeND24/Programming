import pygame,sys
from buttons import *
from settings import *
import text

_set=settings()
clock=pygame.time.Clock()


#ф-ция отрисовки всех предметов на экране
def screen_draw(sc,buttons,sys_butt,\
inputbox,nameteam):
	click=False
	
	for event in pygame.event.get():
		#проверка на выход из приложения
		if event.type==pygame.QUIT:
			sys.exit()
		elif event.type==\
		pygame.MOUSEBUTTONDOWN:
			#проверка, кликнута ли левая кнопка мыши
			if event.button==1:
				click=True
		inputbox[0].check_event(event) #проверка на ввод текста в поле ввода
	
	sc.blit(_set.bg_game,(0,0)) #отрисовка картинки заднего фона
	
	#отрисовка тем заданий и кол-ва очков
	text.draw_theme(sc)
	if _set.butt_count>0:
		text.draw_score(sc,nameteam,\
		_set.score,_set.Next)
	
	#отрисовка всех кнопок 100,200,..,500 на экране
	for i in range(len(buttons)):
		buttons[i].draw(\
		buttons[i].imgInact)
	
	#отрисовка кнопки "Выход" справа внизу экрана	
	sys_butt[4].draw(\
	sys_butt[4].imgInact)
	text.draw_textbutt(sc,2)
	
		
	#если игрок не находится в вопросе и не нажал на любую кнопку
	#то при наведении на кнопки они подсвечиваются
	if _set.check_exit==False:
		if _set.question_act\
		==False:	
			if click==False:
				for i in range(\
				len(buttons)):
					if buttons[i].\
					check_pos()==1:
						buttons[i].draw(\
						buttons[i].imgAct)
						
				if sys_butt[4].\
				check_pos()==1:
					sys_butt[4].draw(\
					sys_butt[4].imgAct)
				text.draw_textbutt(sc,2)	
			
			
#если вопрос неактивен, но нажал на левую кнопку мыши, то считывается положение курсора
#если нажата кнопка, то вопрос становится активным и нажатая кнопка удаляется из массива кнопок
			else:
				_set.time_out[0]=_set.time_out[1]
				for i in range(len(buttons)):
					if (buttons[i].check_pos()==1) \
					and (buttons[i].imgInact\
					!=buttons[i].imgAct):
						_set.question_act=True
						buttons[i].imgInact=\
						buttons[i].imgAct=_set.redbutton
						_set.quest_num=i						
			
			#Если нажата правая нижняя кнопка, то игра спрашивает: выходить или нет			
			if (sys_butt[4].check_pos()==1) \
			and (click==True):
				_set.check_exit=True
			else:
				_set.check_exit=False	
			
	else:
		sc.blit(_set.quest_win,(100,100)) #отрисовка окна уведомления о выходе
		text.draw_exit(sc) #текста предупреждения
		
		sys_butt[5].draw(sys_butt[5].imgInact) #кнопка "уверен"
		sys_butt[6].draw(sys_butt[6].imgInact) #кнопка "не уверен"
		
		#проверка на попадание курсора на кнопки
		for i in range(5,7):
			if sys_butt[i].check_pos()==1:
				sys_butt[i].draw(sys_butt[i].imgAct)
		text.draw_textbutt(sc,3) #надпись на кнопке "уверен"
		text.draw_textbutt(sc,4) #надпись на кнопке "не уверен"
		
		
		if click==True:
			#если нажата кнопка "уверен"
			#то происходит выход в главное меню и очистка всех параметров
			if sys_butt[5].check_pos()==1:
				
				_set.butt_count=25
				_set.score=[0,0]
				_set.Next=0
				_set.check_exit=False
				inputbox[1].k=inputbox[2].k=0
				return True
			
			#если нажата кнопка "не уверен"
			#то происходит выход из окна уведомления о выходе
			elif sys_butt[6].check_pos()==1:
				_set.check_exit=False
	
	
	#если вопрос активен, то отрисовывается экран вопроса, текст вопроса и окно ввода			
	if _set.question_act==True:
		sc.blit(_set.quest_win,(100,100))
		text.draw_quest(sc,_set.quest_num)
		inputbox[0].draw()
		
		#отрисовка счётчика времени для ответа на вопрос
		sc.blit(_set.font[1].render(\
		'Осталось времени: %r'%int(\
		_set.time_out[0]),1,\
		(255, 109, 105)),(300,105))
		if _set.Answer==None:
			_set.time_out[0]-=0.045
		#если время вышло, то вопрос закрывается и ходит другая команда
		if _set.time_out[0]<=0:
			_set.question_act=False
			_set.butt_count-=1
			if _set.Next==0:
				_set.Next=1
			else:
				_set.Next=0
		
		
		#рисование кнопок пропуска вопроса, проверки ответа и текстов на них
		for i in range(2):
			sys_butt[i].draw(sys_butt[i]\
			.imgInact)
			if sys_butt[i].check_pos()\
			==1:
				sys_butt[i].draw(\
				sys_butt[i].imgAct)
					
		text.draw_textbutt(sc,0)
		text.draw_textbutt(sc,1)
		
		
		if _set.Answer==None:
			if click==True:
				#если нажата кнопка "пропустить", то вопрос закрывается и кол-во очков не увеличивается
				if sys_butt[0].check_pos()==1:
					inputbox[0].update()
					inputbox[0].k=0
					_set.question_act=False
					_set.butt_count-=1
					if _set.Answer==False:
						_set.Answer=None
						_set.delay=0
					
					if _set.Next==0:
						_set.Next=1
					else:
						_set.Next=0
					
				#если нажата кнопка "проверить", то происходит проверка введённого ответа с заданным
				#если ответ правильный, то окно вопроса закрывается, окно ввода очищается и кол-во очков увеличивается
				#если ответ неправильный-окно ввода очищается
				if sys_butt[1].check_pos()\
				==1:
					if inputbox[0].text==\
					text.answer[_set.quest_num]:
						_set.score[_set.Next]=\
						_set.score[_set.Next]+\
						buttons[_set.\
						quest_num].score
						inputbox[0].update()
						inputbox[0].k=0
						_set.butt_count-=1
						_set.Answer=True		#ответ правильный
				
						if _set.Next==0:
							_set.Next=1
						else:
							_set.Next=0			
					else:
						inputbox[0].update()
						_set.Answer=False		#ответ неправильный
						
		else:	
			#если ответ правильный, то высвечивается соответствующее окно на некоторое время	
			if _set.Answer==True:
				_set.delay+=1
				if _set.delay<=45:
					#пока не вышло определённое время показывается уведомление
					sc.blit(_set.AnswerTrueFalse1,\
					(200,200))
				else:
					_set.question_act=False
					_set.delay=0
					_set.Answer=None
			#если ответ неправильный, то высвечивается соответствующее окно	на некоторое время	
			elif _set.Answer==False:
				_set.delay+=1
				if _set.delay<=45:
					#пока не вышло определённое время показывается уведомление
					sc.blit(_set.AnswerTrueFalse2,\
					(200,200))
				else:
					_set.delay=0
					_set.Answer=None

	#если все кнопки с вопросами были нажаты и ни один вопрос не активен, то появляется окно конца игры и надпись с кол-вом очков
	if _set.butt_count==0 and \
	_set.question_act==False:
		sc.blit(_set.quest_win,(100,100))
		text.end_game(sc,nameteam\
		,_set.score)
		
		sys_butt[2].draw(\
		sys_butt[2].imgInact)
		if sys_butt[2].check_pos()\
		==1:
			sys_butt[2].draw(\
			sys_butt[2].imgAct)
		text.draw_textbutt(sc,5)
		
		#если кнопка активна и нажата, то происходит выход в меню
		if (click==True) and \
		(sys_butt[2].check_pos()==1):
			_set.butt_count=25
			_set.score=[0,0]
			_set.Next=0
			inputbox[1].k=inputbox[2].k=0
			return True
			
