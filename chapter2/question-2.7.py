from PIL import Image
import os

#installed image magic to get this to work
def displayImage(img):
    os.system(f"imdisplay {img}")

im = Image.open("red.ico")

im = im.convert("RGB")

# get rgba values for each x, y index
impixels = im.load()
