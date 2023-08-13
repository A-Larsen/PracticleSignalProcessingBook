from PIL import Image
import random
import math
import os

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

    def fillRandomPixels(self, percentage):
        p =  int(self.number_of_pixels * (percentage / 100))
        for i in range(0, self.number_of_pixels):
            n = math.floor(random.random() * self.number_of_pixels)
            if i == p:
                pass


im = Im("bike.jpg")
# percentage = 90
# print(im.getPixelPercentage(percentage))
# rand_pixels = generateRandom(im.getPixelPercentage(percentage), im.number_of_pixels)
# print(random.random())

# def callback(x, y, i, pixel_number):
#     if i in rand_pixels:
#         im.pixels[x, y] = (0, 0, 0, 255)
#         print("yep")
#     else:
#         print("nope")

# im.loop(callback)

im.displayImage("out.png")

