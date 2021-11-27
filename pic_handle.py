import os
import sys
import random

from PIL import ImageFont, ImageDraw, Image
import cv2
import numpy as np

def get_argv(this, text, find_text):
    if text.find(find_text) == -1:
        return this
    else:
        return text[text.find(find_text) + len(find_text):]

def image_handle(path, *argv):
    text = "".join(argv)
    dir = os.listdir(path)
    pic = random.randint(0, len(dir))
    save_file_name = "image0.png"

    size = 48
    shadow = 2
    R, G, B = 255, 255, 255
    text_posX, text_posY = 0.1, 0.05

    split = text.split(";")
    text = split[0]
    for row in split:
        size = get_argv(size, row, "size=")
        pic = get_argv(pic, row, "pic=")
        text_posX = get_argv(text_posX, row, "X=")
        text_posY = get_argv(text_posY, row, "Y=")
        shadow = get_argv(shadow, row, "shadow=")
        R = get_argv(R, row, "R=")
        G = get_argv(G, row, "G=")
        B = get_argv(B, row, "B=")
        save_file_name = get_argv(save_file_name, row, "name=")

    save_file_name =  "img/" + save_file_name

    img = cv2.imread(path + '/' + dir[int(pic)])
    height, width, channels = img.shape

    img_pil = Image.fromarray(img)
    draw = ImageDraw.Draw(img_pil)
    x, y = width * float(text_posX), height * float(text_posY)
    font = ImageFont.truetype("./蘋方-繁.otf", int(size))
    shadowcolor = (0, 0, 0)

    draw.text((x - int(shadow), y), text, font=font, fill=shadowcolor)
    draw.text((x + int(shadow), y), text, font=font, fill=shadowcolor)
    draw.text((x, y - int(shadow)), text, font=font, fill=shadowcolor)
    draw.text((x, y + int(shadow)), text, font=font, fill=shadowcolor)

    draw.text((x - int(shadow), y - int(shadow)), text, font=font, fill=shadowcolor)
    draw.text((x + int(shadow), y - int(shadow)), text, font=font, fill=shadowcolor)
    draw.text((x - int(shadow), y + int(shadow)), text, font=font, fill=shadowcolor)
    draw.text((x + int(shadow), y + int(shadow)), text, font=font, fill=shadowcolor)

    draw.text((x, y), text, font=font, fill=(int(R), int(G), int(B)))

    img = np.array(img_pil)
    cv2.imwrite(save_file_name, img)

def main():
    path = "D:\\source\\discord_bot\\discord_bot\\default_save\\rank_daily"
    f = open('./ofs.txt', 'r', encoding='utf8')
    image_handle(path, f.read())

main()

