import pygame

from .board import Board

class Game:
    def __init__(self, window: pygame.Surface):
        self.window = window

        self.board = Board()
        self.turn_num = 0

    def run(self):
        running = True

        while running:
            lmb_pressed = False

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                    return

                else:
                    if event.type == pygame.KEYDOWN:
                        if (event.key == pygame.K_q and pygame.key.get_mods() & pygame.KMOD_CTRL) or event.key == pygame.K_ESCAPE:
                            running = False
                            return

                    if event.type == pygame.MOUSEBUTTONDOWN:
                        if event.button == 1: lmb_pressed = True

            self.window.fill((0, 0, 0))

            self.board.update(self.turn_num, lmb_pressed)
            self.board.draw(self.window)

            pygame.display.update()
