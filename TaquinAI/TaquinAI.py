# q learning with table
import numpy as np
from Game import *
#import os
#os.environ["CUDA_VISIBLE_DEVICES"]="-1"    
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Activation
from tensorflow.keras.optimizers import RMSprop, Adam, SGD
from tensorflow.keras.layers import LeakyReLU
#from tensorflow.python.client import device_lib
import random
import time
import copy
from threading import Thread

from collections import deque

class Trainer:
    def __init__(self, name=None, learning_rate=0.001, epsilon_decay=0.9999, batch_size=30, memory_size=3000):
        self.state_size = 16
        self.action_size = 4
        self.gamma = 0.9
        self.epsilon = 1.0
        self.epsilon_min = 0.01
        self.epsilon_decay = epsilon_decay
        self.learning_rate = learning_rate
        self.memory = deque(maxlen=memory_size)
        self.batch_size = batch_size
        
        self.name = name
        if name is not None and os.path.isfile("model-" + name):
                model = load_model("model-" + name)
        else:
            model = Sequential()
            model.add(Dense(16, input_dim=self.state_size, activation='relu'))
            model.add(Dense(16, activation='relu'))
            model.add(Dense(16, activation='relu'))
            model.add(Dense(8, input_dim=self.state_size/2, activation='relu'))
            model.add(Dense(8, activation='relu'))
            model.add(Dense(self.action_size, activation='linear'))
            model.compile(loss='mse', optimizer=Adam(lr=self.learning_rate))
        
        self.model = model
        
    def decay_epsilon(self):
        self.epsilon *= self.epsilon_decay
    
    def get_best_action(self, state, rand=True):

        if rand and np.random.rand() <= self.epsilon:
            # The agent acts randomly
            return random.randrange(self.action_size)
        
        # Predict the reward value based on the given state
        state = np.reshape(state, (-1, 16))
        act_values = self.model.predict(np.array(state))

        # Pick the action based on the predicted reward
        action =  np.argmax(act_values[0])  
        return action

    def remember(self, state, action, reward, next_state, done):
        self.memory.append([state, action, reward, next_state, done])

    def replay(self, batch_size):
        batch_size = min(batch_size, len(self.memory))

        minibatch = random.sample(self.memory, batch_size)

        inputs = np.zeros((batch_size, self.state_size))
        outputs = np.zeros((batch_size, self.action_size))

        for i, (state, action, reward, next_state, done) in enumerate(minibatch):
            state = np.reshape(state, (-1, 16))
            next_state = np.reshape(next_state, (-1, 16))
            target = self.model.predict(state)[0]
            if done:
                target[action] = reward
            else:
                target[action] = reward + self.gamma * np.max(self.model.predict(next_state))

            inputs[i] = state
            outputs[i] = target

        return self.model.fit(inputs, outputs, epochs=1, verbose=0, batch_size=batch_size)

    def save(self, id=None, overwrite=False):
        name = 'model'
        if self.name:
            name += '-' + self.name
        else:
            name += '-' + str(time.time())
        if id:
            name += '-' + id
        self.model.save(name, overwrite=overwrite)

def train(episodes, trainer, wrong_action_p, alea, collecting=False, snapshot=5000):
    batch_size = 32
    g = Game(4, 4, wrong_action_p, alea=alea)
    counter = 1
    scores = []
    global_counter = 0
    losses = [0]
    epsilons = []
    n_agent = 4
    threads = []
    results = [() for x in range(n_agent)]

    # we start with a sequence to collect information, without learning
    if collecting:
        collecting_steps = 10000
        print("Collecting game without learning")
        steps = 0
        while steps < collecting_steps:
            state = g.reset()
            done = False
            while not done:
                steps += 1

                actions = g.get_random_action(n_agent)
                for ii in range(n_agent):
                    tg = copy.deepcopy(g)
                    process = Thread(target=tg.moveTry, args=[actions[ii], results, ii])
                    process.start()
                    threads.append(process)
                for process in threads:
                    process.join()

                next_state = results[0][0]
                reward = results[0][1]
                done = results[0][2]
                action = results[0][3]
                for obj in results:
                    if obj[2] == True:
                        action = obj[3]
                        done = obj[2]
                        reward = obj[1]
                        next_state = obj[0]
                    elif obj[1] > reward:
                        action = obj[3]
                        done = obj[2]
                        reward = obj[1]
                        next_state = obj[0]
                g.move(action)
                trainer.remember(state, action, reward, next_state, done)
                state = next_state

    print("Starting training")  
    global_counter = 0
    for e in range(episodes+1):
        state = g.generate_game()
        score = 0
        done = False
        steps = 0
        while not done:
            steps += 1
            global_counter += 1
            actions = g.get_random_action(n_agent)
            trainer.decay_epsilon()

            for ii in range(n_agent):
                tg = copy.deepcopy(g)
                process = Thread(target=tg.moveTry, args=[actions[ii], results, ii])
                process.start()
                threads.append(process)
            for process in threads:
                process.join()

            next_state = results[0][0]
            reward = results[0][1]
            done = results[0][2]
            action = results[0][3]
            for obj in results:
                if obj[2] == True:
                    action = obj[3]
                    done = obj[2]
                    reward = obj[1]
                    next_state = obj[0]
                elif obj[1] > reward:
                    action = obj[3]
                    done = obj[2]
                    reward = obj[1]
                    next_state = obj[0]
	   
            g.move(action)
            score += reward
            trainer.remember(state, action, reward, next_state, done)  # ici on enregistre le sample dans la mémoire
            state = next_state
            if global_counter % 100 == 0:
                l = trainer.replay(batch_size)   # ici on lance le 'replay', c'est un entrainement du réseau
                losses.append(l.history['loss'][0])
            if done:
                scores.append(score)
                epsilons.append(trainer.epsilon)
            if steps > 200:
                break
        if e % 25 == 0:
            g.print()
            print("episode: {}/{}, moves: {}, score: {}, epsilon: {}, loss: {}"
                  .format(e, episodes, steps, score, trainer.epsilon, losses[-1]))
        if e > 0 and e % snapshot == 0:
            trainer.save(id='iteration-%s' % e)
    return scores, losses, epsilons

def main():
  #print(device_lib.list_local_devices())
  trainer = Trainer(learning_rate=0.001, epsilon_decay=0.999995)
  scores, losses, epsilons = train(10000, trainer, True, True, snapshot=1000)
  
if __name__== "__main__":
  main()