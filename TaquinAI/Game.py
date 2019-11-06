import random as rd
import math
import numpy as np

class Game:
    ACTION_UP = 0
    ACTION_LEFT = 1
    ACTION_DOWN = 2
    ACTION_RIGHT = 3
    ACTION_END = 4

    ACTIONS = [ACTION_UP, ACTION_LEFT, ACTION_DOWN, ACTION_RIGHT]

    ACTION_NAMES = ["UP", "LEFT", "DOWN", "RIGHT"]

    num_actions = len(ACTIONS)

    def __init__(self, x, y, alea=False):
        self.x = x
        self.y = y
        self.alea = alea
        self.map = self.generate_game()
        self.posX = self.getPos(np.reshape(self.map, (-1, 4)), 0)[0][0]
        self.posY = self.getPos(np.reshape(self.map, (-1, 4)), 0)[1][0]
        #self.mapD = self.generateDistMap()

    def getPos(self, liste: np.array, item: int):
        result = np.where(liste == item)
        return result[0], result[1]

    def getValue(self, liste: np.array, x, y):
        return liste[x,y]

    def get_random_action(self):
        return rd.randrange(0, 4)

    def _get_state(self):
        return self.map

    def _get_grille(self, x, y):
        grille = [
            [0] * self.x for i in range(self.y)
        ]
        grille[self.posX][self.posY] = 1
        return grille

    def generate_game(self):
        map = []
        for i in range(self.x*self.y):
            map += [i]
        
        if self.alea:
            for i in range(self.x*self.y):
                save = rd.randrange(0, self.x*self.y, 1)
                save_1 = rd.randrange(0, self.x*self.y, 1)
                map[save],map[save_1] = map[save_1],map[save]
        a = np.array(map)
        return a 
    
    def reset(self):
        self.map = self.generate_game()
        self.posX = self.getPos(np.reshape(self.map, (-1, 4)), 0)[0][0]
        self.posY = self.getPos(np.reshape(self.map, (-1, 4)), 0)[1][0]
        #self.mapD = self.generateDistMap()
        return self.map

    def vMove(self, OPosX, OPosY, PosX, PosY):
        self.map = np.reshape(self.map, (-1, 4))
        self.map[OPosX][OPosY], self.map[PosX][PosY] = self.map[PosX][PosY], self.map[OPosX][OPosY]
        self.map = self.map.flatten()
        #self.mapD = self.generateDistMap()

    def move(self, action, move):
        """
        takes an action parameter
        :param action : the id of an action
        :return ((state_id, end, hole, block), reward, is_final, actions)
        """
        
        OPosX = self.posX
        OPosY = self.posY

        if action == self.ACTION_UP and self.posX > 0 :
            self.posX -= 1
            self.vMove(OPosX, OPosY, self.posX, self.posY)
        elif action == self.ACTION_DOWN and self.posX < self.x - 1:
            self.posX += 1
            self.vMove(OPosX, OPosY, self.posX, self.posY)
        elif action == self.ACTION_LEFT and self.posY > 0:
            self.posY -= 1
            self.vMove(OPosX, OPosY, self.posX, self.posY)
        elif action == self.ACTION_RIGHT and self.posY < self.y - 1:
            self.posY += 1
            self.vMove(OPosX, OPosY, self.posX, self.posY)
        elif action == self.ACTION_END:
            return self.map, 0, True
        else:
            return self.map, 0, False

        score=self.computeScore()
        win = self.checkWin()
        if win:
            score+=100
            score += 710-(10*move)

        return self.map, score, win

    def checkWin(self):
        win = 0
        base_table = np.array([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0])
        for x in range(len(base_table)):
            if base_table[x] == self.map[x]:
                win += 1
        if win == len(base_table):
            return True
        else:
            return False


    def generateDistMap(self):
        base_table = np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,0]])
        distmap = np.reshape(np.zeros(16), (-1, 4))
        for i in self.map:
            if i != 0:
                x1 = self.getPos(np.reshape(self.map, (-1, 4)), i)[0]
                y1 = self.getPos(np.reshape(self.map, (-1, 4)), i)[1]
                x2 = self.getPos(base_table, i)[0]
                y2 = self.getPos(base_table, i)[1]
                distmap[x1, y1] =  math.sqrt(math.pow((x2-x1),2)+math.pow((y2-y1),2))
        return distmap

    def computeScore(self):
        score = 0
        mpD = self.map
        for i in range(len(mpD)-1):
            if mpD[i]+1 == mpD[i+1]:
                score += 1
        return score - 1


    def print(self):
        str = ""
        nmap = np.reshape(self.map, (-1,4))
        for i in range(self.x):
            for j in range(self.y):
                if i == self.posX and j == self.posY:
                    str = str + "x" + "{:02d}".format(nmap[i,j]) + " "
                else:
                    str += " {:02d}".format(nmap[i,j]) + " "
            str += "\n"
        print(str)
