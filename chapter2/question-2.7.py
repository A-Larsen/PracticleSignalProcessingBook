from PIL import Image
import random
import math
import os


class Im():

    "Wraper for the Image class"

    def __init__(self, image_file:str):
        im = Image.open(image_file)
        im = im.convert("RGB")
        self.number_of_pixels = im.size[0] * im.size[1]
        self.pixels = im.load()
        self.size = im.size
        self.save = im.save

    def displayImage(self, output_file:str) -> None:
        "saves image to specified file and displays it with image magic"
        self.save(output_file)
        os.system(f"imdisplay {output_file}")

    def fillRandomPixels(self, percentage:int, color:tuple) -> None:
        """
        fill random pixel black based off a percentage. You can go over 100
        percent
        """
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

    def copyRandomNeighborLeft(self, percentage:int) -> None:
        """
        selects a random pixel and copies it to the left a percentage number of
        times. You can go over 100 percent
        """
        i = 0
        p = math.floor(self.number_of_pixels * (percentage / 100))
        while i < p:
            rx = math.floor(random.random() * self.size[0])
            ry = math.floor(random.random() * self.size[1])
            left_pixel = (rx - 1, ry)
            if (left_pixel[0] - 1 > 0):
                self.pixels[left_pixel] = self.pixels[rx, ry]
            else:
                continue
            i += 1

    def copyRandomNerestNeighbor(self, percentage:int) -> None:
        """
        selects a random pixel and copies it to the left, right, top and bottom
        a percentage number of times. You can go over 100 percent
        """
        i = 0
        p = math.floor(self.number_of_pixels * (percentage / 100))
        while i < p:
            x = math.floor(random.random() * self.size[0])
            y = math.floor(random.random() * self.size[1])
            pixel = self.pixels[x, y]
            neighbors = {
                    "left":  (x - 1, y),
                    "top":  (x, y + 1),
                    "right":  (x + 1, y),
                    "bottom":  (x, y - 1)
            }
            for key in neighbors.keys():
                n = neighbors[key]
                x_in_bounds = (n[0] >= 0 and 
                               n[0] < self.size[0])
                y_in_bounds = (n[1] >= 0 and 
                               n[1] < self.size[1])
                if  x_in_bounds and y_in_bounds:
                    self.pixels[n] = pixel
                else:
                    continue
            i += 1


im = Im("bike.jpg")
# im.fillRandomPixels(300, (0, 0, 0, 255))
# im.fillPercentage(20, (0, 0, 0, 255))
# im.copyRandomNeighborLeft(3000)
im.copyRandomNerestNeighbor(120)
im.displayImage("out.png")

