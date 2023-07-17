//
//  mission.h
//  ninja
//
//  Created on 17.07.2023.
//  Copyright © 2023 devil. All rights reserved.
//

#ifndef mission_h
#define mission_h

///////////////////////////////////НОМЕР МИССИИ//////////////////////////////////
int getCurrentMission(int x)//ф-ция номера миссия, которая меняет номер миссии, в зависимости от координаты игрока Х (сюда будем передавать эту координату)
{
 int mission = 0;
 if ((x>0) && (x<600)) { mission = 0; } //знакомим игрока с игрой
 if (x>400) { mission = 1; } //игрок на первой миссии
 if (x>700) { mission = 2; }//2ой
 if (x>2200) { mission = 3; }//и тд
 
 return mission;//ф-ция возвращает номер миссии
}
 
 
 
/////////////////////////////////////ТЕКСТ МИССИИ/////////////////////////////////
std::string getTextMission(int currentMission) {
 
 std::string missionText = "";//текст миссии и его инициализация
 
 switch (currentMission)//принимается номер миссии и в зависимости от него переменной missionText присваивается различный текст
 {
 case 0: missionText = "\nНачальный этап и \nинструкции к игре"; break;
 case 1: missionText = "\nMission 1\n\nВот твоя первая\n миссия, на\n этом уровне \nтебе стоит опасаться\n ... бла-бла-бла ..."; break;
 case 2: missionText = "\nMission 2\n Необходимо решить\n логическую задачку,\n чтобы пройти дальше "; break;
 case 3: missionText = "\nИ так далее \nи тому подобное....."; break;
 }
 
 return missionText;//ф-ция возвращает текст
};

#endif /* mission_h */
