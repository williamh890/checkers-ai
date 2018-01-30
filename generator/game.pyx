# game.pyx
# game class to manage c++ board and python gui
# Author: Hal DiMarchi

from gui import Board_Gui

class Game(object):
  def __init__(self):
    self.gui = Board_Gui()
    
