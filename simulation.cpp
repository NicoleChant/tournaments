#include <iostream>
#include <random>
#include <map>
#include <fstream>

double getRandomFloat(){
  std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_real_distribution<> dist(0, 1);
  return dist(e2);
}

int getRandomInteger(int minNum, int maxNum){
  double roll = getRandomFloat();
  double total=1.0/((double) maxNum);
  for(int i=minNum; i<maxNum; ++i){
    if(roll <= (1+i)*total){
      return i+1;
    }

  }

  return -1;
}

struct Player{

  std::string name;
  double health;
  double damage;
  double armor;
  double hitChance;
  double maxHealth;
  Player* targetPtr = nullptr;

  double _applyDamage(){
    // std::cout << dist(e2) << " " << std::endl;
    return getRandomFloat() < hitChance ? (1-reducedDamage(targetPtr->armor)) * damage : 0;
  }

  double reducedDamage(double a){
    return a >= 0 ? a/(a+100.0) : 0.0;
  }

  void applyDamage(){
    if (targetPtr->isAlive()){
      targetPtr->health -= _applyDamage();
      // std::cout << targetPtr->name << " suffered damage!" << std::endl;
    }
    else{
      std::cout << "Target is already dead!" << std::endl;
    }
  }

  bool isAlive(){
    return health>0;
  }

  bool restore(){
    health = maxHealth;
  }

};

void saveSimResults(){};


int main(){
  std::cout << "Hello!" << std::endl;

  // initializing
  Player* firstPlayer = new Player;
  firstPlayer->name = "Alice";

  Player* secondPlayer = new Player;
  secondPlayer->name = "Bob";

  firstPlayer->targetPtr = secondPlayer;
  secondPlayer->targetPtr = firstPlayer;

  std::cout << "Player " << firstPlayer->name << " vs. Player " << firstPlayer->targetPtr->name << std::endl;

  int epochs;
  std::cin >> epochs;
  int rounds;
  double simResults[epochs][10];

  for(int i=0; i<epochs; ++i){
    firstPlayer->hitChance = getRandomFloat();
    secondPlayer->hitChance = getRandomFloat();
    firstPlayer->health = getRandomInteger(2,100);
    secondPlayer->health = getRandomInteger(2,100);
    firstPlayer->maxHealth = firstPlayer->health;
    secondPlayer->maxHealth = secondPlayer->health;
    firstPlayer->armor = getRandomInteger(0,20);
    secondPlayer->armor = getRandomInteger(0,20);
    firstPlayer->damage = getRandomInteger(1,10);
    secondPlayer->damage = getRandomInteger(1,10);

    rounds=0;
    while(firstPlayer->isAlive() && secondPlayer->isAlive()){
      rounds++;
      if(rounds%2==1){
        firstPlayer->applyDamage();
      }
      else{
        secondPlayer->applyDamage();
      }
    }

    simResults[i][0] = firstPlayer->hitChance;
    simResults[i][1] = firstPlayer->maxHealth;
    simResults[i][2] = firstPlayer->armor;
    simResults[i][3] = firstPlayer->damage;
    simResults[i][4] = secondPlayer->hitChance;
    simResults[i][5] = secondPlayer->maxHealth;
    simResults[i][6] = secondPlayer->armor;
    simResults[i][7] = secondPlayer -> damage;
    simResults[i][8] = rounds;
    simResults[i][9] = rounds%2;

    firstPlayer->restore();
    secondPlayer->restore();
    }

  std::cout << "Simulation Finished!" << "\n";

  //save results
  std::string filename="simieResults.csv";
  std::ofstream file(filename);
  std::string header="hitChanceP1,healthP1,armorP1,damageP1,hitChanceP2,healthP2,armorP2,damageP2,rounds,winner";

  file << header << std::endl;

  for(int i=0; i<epochs; ++i){
    for(int j=0; j<10; ++j){
      file << simResults[i][j] << ",";
    }
    file << std::endl;
  }

  file.close();

  delete firstPlayer;
  delete secondPlayer;

  return 0;
}
