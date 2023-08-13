from PIL import Image
import random
import math
import os
import time

class Im():
    """
    Wraper for the Image class
    """
    def __init__(self, image_file:str):
        self.im = Image.open(image_file)
        self.im = self.im.convert("RGB")
        self.pixels = self.im.load()
        self.number_of_pixels = self.im.size[0] * self.im.size[1]

    def displayImage(self, output_file) -> None:
        self.im.save(output_file)
        os.system(f"imdisplay {output_file}")

    def fillRandomPixels(self, percentage, color):
        i = 0
        for y in range(0, self.im.size[1]):
            for x in range(0, self.im.size[0]):
                wrap = i % 100
                if wrap < percentage:
                    self.pixels[x, y] = color 
                i += 1


im = Im("bike.jpg")
print(im.im.size[0], im.im.size[1])
im.fillRandomPixels(20, (0, 0, 0, 255))

im.displayImage("out.png")

