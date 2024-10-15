#!/usr/bin/env python3

from PIL import Image
import numpy as np
import os


TEMPLATE_FILE = "icon16_template.png"
BASE_SIZE = 16


FAVICON_SIZES = [16, 24, 32, 48, 64, 128, 256]
WEB_ICONS = [180, 196, 512]
ANDROID = [("mipmap-mdpi", 48),
           ("mipmap-hdpi", 72),
           ("mipmap-xhdpi", 96),
           ("mipmap-xxhdpi", 144),
           ("mipmap-xxxhdpi", 192)]


template_pil = Image.open(TEMPLATE_FILE)
template = np.asarray(template_pil)


def create_icon(size: int):
    scale = np.floor(size / BASE_SIZE)
    img = template.repeat(scale, axis=0).repeat(scale, axis=1)

    center = img.shape[0] // 2
    start = center-size//2
    end = center+size//2
    icon = img[start:end, start:end, :]
    return np.array(icon)


print("creating favicon.ico")
FAVICON_ICONS = []
for size in FAVICON_SIZES:
    icon = create_icon(size)
    FAVICON_ICONS.append(Image.fromarray(icon))

FAVICON_ICONS[-1].save("favicon.ico", append_images=FAVICON_ICONS, format="ICO")


for size in WEB_ICONS:
    dst = "icon%i.png" % size
    print("creating", dst)

    icon = create_icon(size)
    icon_pil = Image.fromarray(icon)
    icon_pil.save(dst, format="PNG")


for dir_n_size in ANDROID:
    dir = dir_n_size[0]
    size = dir_n_size[1]
    dst = dir + "/ic_launcher.png"
    print("creating", dst)

    try:
        os.makedirs(dir)
    except:
        pass

    icon = create_icon(size)
    icon_pil = Image.fromarray(icon)
    icon_pil.save(dst, format="PNG")


print("fin")
