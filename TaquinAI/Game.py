import random as rd
import numpy as np

class Game:
    ACTION_UP = 0
    ACTION_LEFT = 1
    ACTION_DOWN = 2
    ACTION_RIGHT = 3
    ACTION_END = 4

    ACTIONS = [ACTION_DOWN, ACTION_LEFT, ACTION_RIGHT, ACTION_UP]

    ACTION_NAMES = ["UP", "LEFT", "DOWN", "RIGHT"]

    num_actions = len(ACTIONS)

    def __init__(self, x, y, wrong_action_p=0.1, alea=False):
        self.x = x
        self.y = y
        self.posX = 0
        self.posY = 0
        self.alea = alea
        self.map = self.generate_game()

    def getPos(self, liste: np.array, item: int):
        result = np.where(liste == item)
        return zip(result[0], result[1])

    def getValue(self, liste: np.array, x, y):
        return liste[x,y]

    def get_random_action():
        return rd.randint(0, 5)

    def _get_state(self):
        return flatten(self._get_grille(self.posX, self.posY))

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
        return np.reshape(a, (-1, 4))
    
    def reset(self):
        map = self.generate_game()

    def move(self, action):
        """
        takes an action parameter
        :param action : the id of an action
        :return ((state_id, end, hole, block), reward, is_final, actions)
        """
        
        #self.counter += 1

        if action not in self.ACTIONS:
            raise Exception("Invalid action")

            distD=self.checkDist()
            if action == self.ACTION_UP and self.posX > 0 :
                self.posX -= 1;
            elif action == self.ACTION_DOWN and self.posX < self.x - 1:
                self.posX += 1;
            elif action == self.ACTION_LEFT and self.posY > 0:
                self.posY -= 1;
            elif action == self.ACTION_RIGHT and self.posY < self.y - 1:
                self.posY += 1;
            elif action == self.ACTION_END:
                return -1;
            else:
                return -1
            distN=self.checkDist()
            win = self.checkWin()
            return self.map, distD - distN, win

    def checkWin(self):
        base_table = np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,0]])
        if self.map == base_table:
            return True
        else:
            return False

    def checkDist(self):
        base_table = np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,0]])
        x1=self.getPos(base_table, getValue(self.posX, self.posY))[0]
        x2=self.posX
        y1=self.getPos(base_table, getValue(self.posX, self.posY))[1]
        y2=self.posY
        return math.sqrt(math.pow((x2-x1),2)+math.pow((y2-y1),2))


    def print(self):
        str = ""
        for i in range(self.x):
            for j in range(self.y):
                if i == self.posX and j == self.posY:
                    str = str + "x" + "{:02d}".format(self.map[i,j]) + " "
                else:
                    str += "{:02d}".format(self.map[i,j]) + " "
            str += "\n"
        print(str)
