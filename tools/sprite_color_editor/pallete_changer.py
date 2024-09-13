from PIL import Image
from pathlib import Path
import os

class ColorChanger:
    def __init__(self, old_col: tuple[int, int, int, int], new_col: tuple[int, int, int, int]):
        self.old_col = old_col
        self.new_col = new_col

    def change(self, directory: str):
        base_path = Path(__file__).parent / directory

        editedImages = set()

        for file_name in os.listdir(base_path):
            img_folder_path = base_path / file_name
            if os.path.isdir(str(img_folder_path)):
                img_path = img_folder_path / sorted(os.listdir(img_folder_path))[1]

                img = Image.open(img_path)
                img = img.convert("RGBA")

                for y in range(img.size[1]):
                    for x in range(img.size[0]):
                        if img.getpixel((x, y)) == self.old_col:
                            if (img_path not in editedImages):
                                editedImages.add(img_path)
                                print(f"img: {img_path.name} was edited.")
                            img.putpixel((x, y), (self.new_col))

                img.save(img_path)

ColorChanger((245, 240, 200, 255), (230, 225, 190, 255)).change("pieces")
