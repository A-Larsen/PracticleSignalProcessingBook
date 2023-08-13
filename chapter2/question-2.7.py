from PIL import Image
import random
import math
import os

class Im():
    """
    Wraper for the Image class
    """
    def __init__(self, image_file:str):
        im = Image.open(image_file)
        im = im.convert("RGB")
        self.number_of_pixels = im.size[0] * im.size[1]
        self.pixels = im.load()
        self.size = im.size
        self.save = im.save

    def displayImage(self, output_file) -> None:
        "saves image to specified file and displays it with image magic"
        self.save(output_file)
        os.system(f"imdisplay {output_file}")

    def blackRandomPixels(self, percentage:int, color:tuple) -> None:
        "fill random pixel black based off a percentage"
        # the random selection seems to be selecting the same pixels multiple
        # times
        i = 0
        p = math.floor(self.number_of_pixels * (percentage / 100))
        while i < p:
            rx = math.floor(random.random() * self.size[0])
            ry = math.floor(random.random() * self.size[1])
            self.pixels[rx, ry] = color 
            i += 1

    def fillPercentage(self, percentage:int, color:tuple) -> None:
        "fill a percentage of the image a specified color"
        i = 0
        for y in range(0, self.size[1]):
            for x in range(0, self.size[0]):
                wrap = i % 100
                if wrap < percentage:
                    self.pixels[x, y] = color 
                i += 1


im = Im("bike.jpg")
im.blackRandomPixels(99, (0, 0, 0, 255))
# im.fillPercentage(99, (0, 0, 0, 255))
im.displayImage("out.png")

