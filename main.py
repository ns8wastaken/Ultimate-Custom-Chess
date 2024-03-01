import pygame

from src.game import Game
from src.settings import Settings

def main():
    pygame.init()
    pygame.display.set_caption('Ultimate Custom Chess')

    window = pygame.display.set_mode((Settings['x_size'] * Settings['tile_size'], Settings['y_size'] * Settings['tile_size']))

    game = Game(window)
    game.run()

    pygame.quit()

if __name__ == '__main__':
    main()
