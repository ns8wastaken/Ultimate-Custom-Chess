import pygame

from .piece import *

class Board:
    def __init__(self):
        from .settings import Settings

        self.tileSize = Settings['tile_size']
        self.x_size = Settings['x_size']
        self.y_size = Settings['y_size']

        self.light_col = Settings['light_tile']
        self.dark_col = Settings['dark_tile']

        self.board: dict[tuple[int, int], Piece] = dict()
        self._setup_pieces()

        self.size = (self.x_size * self.tileSize, self.y_size * self.tileSize)
        self.board_surf = pygame.Surface(self.size)
        self._make_bg()

        self.lmb_pressed = False

        self.hovering_square_moves = None
        self.selected_square = None
        self.selected_piece_moves = None

        self.selected_surf = pygame.Surface((1, 1))
        self.move_surf = pygame.Surface((1, 1))
        self._setup_surfs()

    def _setup_surfs(self):
        # Set up surface for selected tiles
        self.selected_surf = pygame.Surface((self.tileSize, self.tileSize))
        self.selected_surf.fill((255, 255, 0))
        self.selected_surf.set_alpha(160)

        # Set possible move location surface
        self.move_surf = pygame.Surface((self.tileSize, self.tileSize))
        self.move_surf.fill((255, 0, 0))
        self.move_surf.set_alpha(150)

    def _setup_pieces(self):
        self.board[(2, 3)] = Snake('white', self.tileSize)
        # Pawns
        for i in range(self.x_size):
            self.board[(i, 1)] = Pawn(color='black', tileSize=self.tileSize)
            self.board[(i, self.y_size - 2)] = Pawn(color='white', tileSize=self.tileSize)

        # Knights
        self.board[(1, 0)] = Knight(color='black', tileSize=self.tileSize)
        self.board[(self.x_size - 2, 0)] = Knight(color='black', tileSize=self.tileSize)
        self.board[(1, self.y_size - 1)] = Knight(color='white', tileSize=self.tileSize)
        self.board[(self.x_size - 2, self.y_size - 1)] = Knight(color='white', tileSize=self.tileSize)

        # Rooks
        self.board[(0, 0)] = Rook(color='black', tileSize=self.tileSize)
        self.board[(self.x_size - 1, 0)] = Rook(color='black', tileSize=self.tileSize)
        self.board[(0, self.y_size - 1)] = Rook(color='white', tileSize=self.tileSize)
        self.board[(self.x_size - 1, self.y_size - 1)] = Rook(color='white', tileSize=self.tileSize)

        # Bishops
        self.board[(2, 0)] = Bishop(color='black', tileSize=self.tileSize)
        self.board[(self.x_size - 3, 0)] = Bishop(color='black', tileSize=self.tileSize)
        self.board[(2, self.y_size - 1)] = Bishop(color='white', tileSize=self.tileSize)
        self.board[(self.x_size - 3, self.y_size - 1)] = Bishop(color='white', tileSize=self.tileSize)

        # Queens
        self.board[(3, 0)] = Queen(color='black', tileSize=self.tileSize)
        self.board[(3, self.y_size - 1)] = Queen(color='white', tileSize=self.tileSize)

        # Kings
        self.board[(4, 0)] = King(color='black', tileSize=self.tileSize)
        self.board[(4, self.y_size - 1)] = King(color='white', tileSize=self.tileSize)

    def _make_bg(self):
        light_square = pygame.Surface((self.tileSize, self.tileSize))
        dark_square = light_square.copy()

        light_square.fill(self.light_col)
        dark_square.fill(self.dark_col)

        for i in range(self.x_size):
            for j in range(self.y_size):
                if (i + j) % 2 == 0:
                    self.board_surf.blit(light_square, (i * self.tileSize, j * self.tileSize))
                else:
                    self.board_surf.blit(dark_square, (i * self.tileSize, j * self.tileSize))

    def coord_to_grid(self, coords: tuple[int, int]) -> tuple[int, int]:
        return (coords[0] // self.tileSize, coords[1] // self.tileSize)

    def grid_to_coord(self, gridPos: tuple[int, int]) -> tuple[int, int]:
        return (gridPos[0] * self.tileSize, gridPos[1] * self.tileSize)

    def select(self, boardPos: tuple[int, int]):
        if (boardPos in self.board) and (boardPos != self.selected_square):
            self.selected_square = boardPos
            self.selected_piece_moves = self.board[self.selected_square].getMoves(self.selected_square, self.board)

        else:
            self.selected_square = None
            self.selected_piece_moves = None

    def move_piece(self, fromPos: tuple[int, int], toPos: tuple[int, int]):
        self.board[toPos] = self.board[fromPos]
        del self.board[fromPos]
        self.board[toPos].moveCount += 1

        self.selected_square = None
        self.selected_piece_moves = None

    def set_hovering_moves(self, mouseBoardPos: tuple[int, int]):
        if self.selected_square == None and mouseBoardPos in self.board:
            self.hovering_square_moves = self.board[mouseBoardPos].getMoves(mouseBoardPos, self.board)
        else:
            self.hovering_square_moves = None

    def _handle_mouse(self, mouseBoardPos: tuple[int, int], lmb_pressed: bool):
        if lmb_pressed:
            if self.selected_piece_moves and mouseBoardPos in self.selected_piece_moves:
                if self.selected_square:
                    self.move_piece(self.selected_square, mouseBoardPos)
            else:
                self.select(mouseBoardPos)

        self.set_hovering_moves(mouseBoardPos)

    def draw(self, screen: pygame.Surface):
        # Draw background
        screen.blit(self.board_surf, (0, 0))

        # Highlight selected square
        if self.selected_square:
            screen.blit(self.selected_surf, self.grid_to_coord(self.selected_square))

        # Show hovering moves
        if self.hovering_square_moves:
            for coords in self.hovering_square_moves:
                screen.blit(self.move_surf, self.grid_to_coord(coords))

        # Show move locations
        if self.selected_piece_moves:
            for coords in self.selected_piece_moves:
                screen.blit(self.move_surf, self.grid_to_coord(coords))

        # Draw pieces
        for coords, piece in self.board.items():
            screen.blit(piece.sprite, self.grid_to_coord(coords))

    def update(self, turn_num: int, lmb_pressed: bool):
        mouseBoardPos = self.coord_to_grid(pygame.mouse.get_pos())

        # Handle cursor
        if mouseBoardPos in self.board:
            pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_HAND)
        else:
            pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_ARROW)

        self._handle_mouse(mouseBoardPos, lmb_pressed)
