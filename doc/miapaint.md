# Mia Paint
Back to [README](../README.md)



## Ideas and old todo's

### Presets
To be activated in the settings, or at first start-up
- Simple
    - basic brushes
    - traditional palette
- Pixelart
    - pixel art settings
    - pixel brushes
    - traditional palette?
- Advanced Drawing
    - basic brushes
    - color picker
    - layers
- Image Editing?

### todo

- github actions update sshsebi debug server?

- github repo for config stuff
    - alias shell gpush, etc.
    - chatgpt greetings


- dbl tap error on webgl?
- "out of memory or smth?"

- Define brushs + dynamics
    - maybe standard file format like gimp?
        - mypaint is parametric only... lets stick with textures + dynamics like in gimp?
- options view for brushs + dynamics
    - hardening
    - hardening_add_by_pressure
    - ...

- gui ideas
    - each view has a settings icon at top left
        - option to remove
            - empty views (class?) have a remove and a select view button?
                - or simple remove...
        - option to add split
            - new view (as scene) with options
                - next to: left, right, botton, top
        - split has a dragable resizer?

- Layers and all on cpu (UImg)
- working layer(s) as RTex

- UImg new version?
    - only unsigned char type?
    - only cpu...
    - UImg_set_tex
        - asserts sizes
    - distance transform
    - select
        - fill
        - replace -> RTex!
        - options for smooth edges | tolerance band
    - minmax
    - some default single pass functions as in RTex, if UImg is needed once
        - add_scaled, etc.
        - normalize


- o_img format?

- u_stroke
    - data
        - pos
        - size
        - ratio
        - angle
        - hardness
        - pressure (alpha)
        - color
    - funcs
        - u_stroke_quad



Back to [README](../README.md)
