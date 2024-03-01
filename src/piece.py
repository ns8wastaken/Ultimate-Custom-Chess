import pygame, os
from typing import Literal

from .settings import Settings

class Piece:
    def __init__(self, color):
        self.color = color

        self.gameOver_onDeath = False

        self.sprite = pygame.Surface((10, 10))

        self.moveCount = 0

    def _coords_in_board(self, coords: tuple[int, int]):
        return (0 <= coords[0] < Settings['x_size']) and (0 <= coords[1] < Settings['y_size'])

    def setSprite(self, sprite_folder_path: str, tileSize: int):
        sprite_folder_path = Settings['piece_sprite_path'] + sprite_folder_path
        black, white = os.listdir(sprite_folder_path)
        if self.color == 'white':
            self.sprite = pygame.image.load(sprite_folder_path + '/' + white).convert_alpha()
        else:
            self.sprite = pygame.image.load(sprite_folder_path + '/' + black).convert_alpha()

        self.sprite = pygame.transform.scale(self.sprite, (tileSize, tileSize))

    def setParams(self, gameOver_onDeath: bool):
        self.gameOver_onDeath = gameOver_onDeath

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], 'Piece']) -> list[tuple[int, int]]:
        moves = []

        return moves


class Pawn(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('pawn', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []

        direction = -1 if self.color == 'white' else 1

        newPos = (currentPos[0], currentPos[1] + direction)
        if newPos not in board and self._coords_in_board(newPos):
            moves.append(newPos)

        left_kill = (newPos[0] - 1, newPos[1])
        if left_kill in board and board[left_kill].color != self.color:
            moves.append(left_kill)

        right_kill = (newPos[0] + 1, newPos[1])
        if right_kill in board and board[right_kill].color != self.color:
            moves.append(right_kill)

        newPos = (currentPos[0], currentPos[1] + direction * 2)
        if self.moveCount == 0 and self._coords_in_board(newPos) and newPos not in board:
            moves.append(newPos)

        return moves


class Knight(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('knight', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []

        x, y = currentPos
        for nx, ny in [(-1, -2), (1, -2), (2, -1), (2, 1), (1, 2), (-1, 2), (-2, 1), (-2, -1)]:
            newPos = (x + nx, y + ny)
            if self._coords_in_board(newPos):
                if newPos not in board:
                    moves.append(newPos)

                elif board[newPos].color != self.color:
                    moves.append(newPos)

        return moves


class Bishop(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('bishop', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []
        x, y = currentPos

        directions = [(-1, -1), (1, -1), (1, 1), (-1, 1)]

        for dx, dy in directions:
            for i in range(1, min(Settings['x_size'], Settings['y_size'])):
                newPos = (x + dx * i, y + dy * i)

                if not self._coords_in_board(newPos):
                    break

                elif newPos not in board:
                    moves.append(newPos)

                elif board[newPos].color != self.color:
                    moves.append(newPos)
                    break

                else: break

        return moves


class Rook(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('rook', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []
        x, y = currentPos

        directions = [(0, -1), (1, 0), (0, 1), (-1, 0)]

        for dx, dy in directions:
            for i in range(1, max(Settings['x_size'], Settings['y_size'])):
                newPos = (x + dx * i, y + dy * i)

                if not self._coords_in_board(newPos):
                    break

                elif newPos not in board:
                    moves.append(newPos)

                elif board[newPos].color != self.color:
                    moves.append(newPos)
                    break

                else: break

        return moves


class Queen(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('queen', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []
        x, y = currentPos

        directions = [(-1, -1), (0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0)]

        for dx, dy in directions:
            for i in range(1, max(Settings['x_size'], Settings['y_size'])):
                newPos = (x + dx * i, y + dy * i)

                if not self._coords_in_board(newPos):
                    break

                elif newPos not in board:
                    moves.append(newPos)

                elif board[newPos].color != self.color:
                    moves.append(newPos)
                    break

                else: break

        return moves


class King(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('king', tileSize=tileSize)
        self.setParams(gameOver_onDeath=True)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []

        x, y = currentPos
        for dx, dy in [(-1, -1), (0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0)]:
            newPos = (x + dx, y + dy)
            if self._coords_in_board(newPos):
                if newPos not in board:
                    moves.append(newPos)

                elif board[newPos].color != self.color:
                    moves.append(newPos)

        # if self.color == 'white':
        if self.moveCount == 0 and (x + 1, y) not in board and (x + 2, y) not in board:
            if (x + 3, y) in board and board[(x + 3, y)].moveCount == 0:
                moves.append((x + 2, y))

        if self.moveCount == 0 and (x - 1, y) not in board and (x - 2, y) not in board and (x - 3, y) not in board:
            if (x - 4, y) in board and board[(x - 4, y)].moveCount == 0:
                moves.append((x - 2, y))

        return moves





class Cubist(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('cubist', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []

        x, y = currentPos
        offsets = [
            (-2, -2), (-1, -2), (0, -2), (1, -2), (2, -2),
            (-2, -1), (2, -1),
            (-2, 0), (2, 0),
            (-2, 1), (2, 1),
            (-2, 2), (-1, 2), (0, 2), (1, 2), (2, 2),
        ]

        for dx, dy in offsets:
            newPos = (x + dx, y + dy)
            if self._coords_in_board(newPos):
                if newPos not in board:
                    moves.append(newPos)

                elif board[newPos].color != self.color:
                    moves.append(newPos)

        return moves


class Observer(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('observer', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = set()

        kingPos = None
        for coords, piece in board.items():
            if isinstance(piece, King):
                kingPos = coords

        x, y = currentPos

        kx = ky = None
        if kingPos:
            kx, ky = kingPos
        newKingPos = None

        for dx, dy in [(-1, -1), (0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0)]:
            newPos = (x + dx, y + dy)

            if kx and ky:
                newKingPos = (kx + dx, ky + dy)

            if self._coords_in_board(newPos):
                if newPos not in board:
                    moves.add(newPos)

                elif board[newPos].color != self.color:
                    moves.add(newPos)

            if newKingPos:
                if self._coords_in_board(newKingPos):
                    if newKingPos not in board:
                        moves.add(newKingPos)

                    elif board[newKingPos].color != self.color:
                        moves.add(newKingPos)

        return list(moves)


class Snake(Piece):
    def __init__(self, color: Literal['white', 'black'], tileSize: int):
        super().__init__(color=color)
        self.setSprite('snake', tileSize=tileSize)

    def getMoves(self, currentPos: tuple[int, int], board: dict[tuple[int, int], Piece]) -> list[tuple[int, int]]:
        moves = []

        x, y = currentPos
        for dx, dy in [
            (0, -2 if self.color == 'white' else 2),
            (-2, 2 if self.color == 'white' else -2),
            (2, 2 if self.color == 'white' else -2)
        ]:
            newPos = (x + dx, y + dy)

            if self._coords_in_board(newPos):
                if newPos in board and board[newPos].color != self.color:
                    moves.append(newPos)

        left_move = (x - 1, y)
        if left_move not in board and self._coords_in_board(left_move):
            moves.append(left_move)

        right_move = (x + 1, y)
        if left_move not in board and self._coords_in_board(right_move):
            moves.append(right_move)

        return moves


__all__ = [
    # Default pieces
    'Piece', 'Pawn', 'Knight', 'Bishop', 'Rook', 'Queen', 'King',

    # Custom pieces
    'Cubist', 'Observer', 'Snake'
]
