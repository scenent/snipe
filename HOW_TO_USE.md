# screen language

you can use `new` and `def` keyword in `screen.txt`.  
`new` keyword used to create a new object and `def` keyword used to define object's properties.  
there's 4 rules that you should follow:

1. you can use `new` keyword after the all of `def` keyword command is finished.
2. you should use 4 spaces instead of 1 tab.
3. you should declare 1 root and 1 scene and 1 camera at least.
4. you can create objects inside the `root`,  `scene`, `vbox`'s scope only.

```python
new "root" # you should make root only once.
    def "window_size" = vec2(1600, 900) # define window's size to 1600 x 900
    def "window_title" = "\"Untitled\"" # define window's title as "Untitled"
    def "current_scene" = "myScene"     # define current scene with its name
    new "scene"
        def "name" = "myScene"          # define scene's name
        def "current_camera" = "myCamera" # define current camera with its name
        new "camera"
            def "name" = "myCamera"     # define camera's name
```

The properties that you can specify for each object can be found below:

> root
> 
> | `name`             | `type`  | `description`                                  |
> | ------------------ | ------- | ---------------------------------------------- |
> | window_position    | vector2 | define window's position                       |
> | window_size        | vector2 | define window's size                           |
> | window_title       | string  | define window's title                          |
> | window_fullscreen  | bool    | define window's fullscreen flag                |
> | window_allow_hidpi | bool    | define window's hidpi flag                     |
> | window_quit_on_esc | bool    | define whether quit game when press escape key |
> | window_target_fps  | float   | define window's target FPS                     |
> | current_scene      | string  | set current scene with its name                |

---

> scene
> 
> | `name`            | `type` | `description`                                                                                           |
> | ----------------- | ------ | ------------------------------------------------------------------------------------------------------- |
> | name              | string | define scene's name                                                                                     |
> | current_camera    | string | set current camera with its name (camera should be created inside of scene's scope)                     |
> | current_dialog    | string | set current dialog with its name (dialog should be created inside of scene's scope)                     |
> | current_choicebox | string | set current choicebox with its name (choicebox should be created inside of scene's scope)               |
> | current_history   | string | set current historycontainer with its name (historycontainer should be created inside of scene's scope) |

---

> camera
> 
> | `name`          | `type`  | `description`                                                                      |
> | --------------- | ------- | ---------------------------------------------------------------------------------- |
> | name            | string  | define camera's name                                                               |
> | position        | vector2 | define camera's position                                                           |
> | zoom            | vector2 | define camera's zoom rate                                                          |
> | modify_movement | bool    | if this value is false, camera does not move well when it is rotated               |
> | offset          | vector2 | define camera's offset (this value effects on camera's rotation and zoom in / out) |
> | rotation        | float   | define camera's rotation                                                           |

---

> sprite
> 
> | `name`       | `type`  | `description`                                                       |
> | ------------ | ------- | ------------------------------------------------------------------- |
> | name         | string  | define sprite's name                                                |
> | tag          | string  | define sprite's tag                                                 |
> | position     | vector2 | define sprite's position                                            |
> | pivot_offset | vector2 | define sprite's pivot offset                                        |
> | rotation     | float   | define sprite's rotation                                            |
> | scale        | vector2 | define sprite's scale                                               |
> | file_path    | string  | assign sprite's file path                                           |
> | pixel_filter | bool    | if this value is true, sprite will rendered without OpenGL's filter |
> | z_order      | int     | define sprite's z-order                                             |
> | visible      | bool    | define sprite's visible                                             |
> | modulate     | vec4    | define sprite's modulate                                            |

---

> vbox (or vboxcontainer)
> 
> | `name`   | `type`  | `description`          |
> | -------- | ------- | ---------------------- |
> | name     | string  | define vbox's name     |
> | position | vector2 | define vbox's position |
> | size     | vector2 | define vbox's size     |
> | z_order  | int     | define vbox's z-order  |

---

> dialog
> 
> | `name`               | `type`  | `description`                       |
> | -------------------- | ------- | ----------------------------------- |
> | name                 | string  | define dialog's name                |
> | namebox_file_path    | string  | assign namebox image's file path    |
> | background_file_path | string  | assign background image's file path |
> | namebox_scale        | vector2 | define namebox image's scale        |
> | background_scale     | vector2 | define background image's scale     |
> | position             | vector2 | define dialog's position            |
> | pivot_offset         | vector2 | define dialog's pivot offset        |
> | rotation             | float   | define dialog's rotation            |
> | namelabel_position   | vector2 | define name label's position        |
> | background_position  | vector2 | define background image's position  |
> | size                 | vector2 | define dialog's size                |
> | namelabel_size       | vector2 | define name label's size            |
> | font_path            | string  | assign dialog's font path           |
> | namelabel_font_path  | string  | assign name label's font path       |
> | speed                | float   | define dialog's text speed          |
> | visible              | bool    | define dialog's visible             |
> | font_size            | int     | define dialog's font size           |
> | namelabel_font_size  | int     | define name label's font size       |
> | font_color           | vector4 | define dialog's font color          |
> | outline_size         | float   | define dialog's glyph outline size  |
> | outline_color        | vector4 | define dialog's glyph outline color |
> | wrap_text            | bool    | define dialog's wrap text flag      |
> | wrap_by_word         | bool    | define dialog's wrap by word flag   |
> | z_order              | int     | define dialog's z-order             |

---

> choicebox
> 
> | `name`        | `type`  | `description`                                        |
> | ------------- | ------- | ---------------------------------------------------- |
> | name          | string  | define choicebox's name                              |
> | position      | vector2 | define choicebox's position                          |
> | pivot_offset  | vector2 | define choicebox's pivot offset                      |
> | rotation      | float   | define choicebox's rotation                          |
> | size          | vector2 | define choicebox's size                              |
> | separation    | float   | define choicebox's separation between of each choice |
> | each_box_size | float   | define choice's each y size                          |
> | font_size     | int     | define choicebox's font size                         |
> | font_path     | string  | assign choicebox's font path                         |
> | font_color    | vector4 | define choicebox's font color                        |
> | outline_size  | float   | define choicebox's glyph outline size                |
> | outline_color | vector4 | define choicebox's glyph outline color               |
> | visible       | bool    | define choicebox's visible flag                      |
> | z_order       | int     | define choicebox's z-order                           |

---

> button
> 
> | `name`      | `type`  | `description`                                                                                          |
> | ----------- | ------- | ------------------------------------------------------------------------------------------------------ |
> | name        | string  | define button's name                                                                                   |
> | position    | vector2 | define button's position                                                                               |
> | rotation    | float   | define button's rotation                                                                               |
> | size        | vector2 | define button's size                                                                                   |
> | font_path   | string  | define button's font path                                                                              |
> | font_size   | int     | define button's font size                                                                              |
> | text        | string  | define button's text                                                                                   |
> | idle_color  | vector4 | define button's color when it's idle state                                                             |
> | hover_color | vector4 | define button's color when it's hover state                                                            |
> | down_color  | vector4 | define button's color when it's down state                                                             |
> | callback    | string  | add button's callback label (you should use label's name and you can write this command several times) |
> | visible     | bool    | define button's visible                                                                                |
> | z_order     | int     | define button's z-order                                                                                |

---

> text_button
> 
> | `name`              | `type`  | `description`                                                                                               |
> | ------------------- | ------- | ----------------------------------------------------------------------------------------------------------- |
> | name                | string  | define text button's name                                                                                   |
> | position            | vector2 | define text button's position                                                                               |
> | pivot_offset        | vector2 | define text button's pivot offset                                                                           |
> | rotation            | float   | define text button's rotation                                                                               |
> | size                | vector2 | define text button's size                                                                                   |
> | font_size           | int     | define text button's font size                                                                              |
> | font_path           | string  | define text button's font path                                                                              |
> | outline_size        | float   | define text button's font outline size                                                                      |
> | text                | string  | define text button's text                                                                                   |
> | idle_outline_color  | vector4 | define text button's outline color when it's idle state.                                                    |
> | idle_color          | vector4 | define text button's font color when it's idle state.                                                       |
> | hover_outline_color | vector4 | define text button's outline color when it's hover state.                                                   |
> | hover_color         | vector4 | define text button's font color when it's hover state.                                                      |
> | down_outline_color  | vector4 | define text button's outline color when it's down state.                                                    |
> | down_color          | vector4 | define text button's font color when it's down state.                                                       |
> | callback            | string  | add text button's callback label (you should use label's name and you can write this command several times) |
> | visible             | bool    | define text button's visible                                                                                |
> | z_order             | int     | define text button's z-order                                                                                |

---

> textedit
> 
> | `name`         | `type`  | `description`                                                          |
> | -------------- | ------- | ---------------------------------------------------------------------- |
> | name           | string  | define textedit's name                                                 |
> | position       | vector2 | define textedit's position                                             |
> | pivot_offset   | vector2 | define textedit's pivot offset                                         |
> | size           | vector2 | define textedit's size                                                 |
> | fast_backspace | bool    | if you set this attribute to true, you can use faster backspace speed. |
> | font_path      | string  | define textedit's font path                                            |
> | font_size      | int     | define textedit's font size                                            |
> | font_color     | vector4 | define textedit's font color                                           |
> | z_order        | int     | define textedit's z-order                                              |

---

> color_rect
> 
> | `name`       | `type`  | `description`                    |
> | ------------ | ------- | -------------------------------- |
> | name         | string  | define color rect's name         |
> | position     | vector2 | define color rect's position     |
> | pivot_offset | vector2 | define color rect's pivot offset |
> | rotation     | float   | define color rect's rotation     |
> | size         | vector2 | define color rect's size         |
> | visible      | bool    | define color rect's visible      |
> | z_order      | int     | define color rect's z-order      |
> | color        | vector4 | define color rect's color        |

---

> hslider
> 
> | `name`           | `type`  | `description`                                                                                           |
> | ---------------- | ------- | ------------------------------------------------------------------------------------------------------- |
> | name             | string  | define hslider's name                                                                                   |
> | position         | vector2 | define hslider's position                                                                               |
> | pivot_offset     | vector2 | define hslider's pivot offset                                                                           |
> | rotation         | float   | define hslider's rotation                                                                               |
> | size             | vector2 | define hslider's size                                                                                   |
> | visible          | bool    | define hslider's visible                                                                                |
> | z_order          | int     | define hslider's z-order                                                                                |
> | percent          | float   | define hslider's progressing percent                                                                    |
> | idle_color       | vector4 | define hslider's grabber color when it's idle state                                                     |
> | hover_color      | vector4 | define hslider's grabber color when it's hover state                                                    |
> | down_color       | vector4 | define hslider's grabber color when it's down state                                                     |
> | grabber_size     | vector2 | define hslider's grabber size                                                                           |
> | callback         | string  | add hslider's callback label (you should use label's name and you can write this command several times) |
> | callback_value   | string  | set callback value to given variable's name                                                             |
> | background_color | vector4 | define hslider's background color                                                                       |

---

> label
> 
> | `name`          | `type`  | `description`                                                                                                                               |
> | --------------- | ------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
> | name            | string  | define label's name                                                                                                                         |
> | text            | string  | define label's text                                                                                                                         |
> | font_path       | string  | define label's font path                                                                                                                    |
> | percent_visible | float   | define label's percent-visible                                                                                                              |
> | color           | vector4 | define label's font color                                                                                                                   |
> | outline_color   | vector4 | define label's font outline color                                                                                                           |
> | outline_size    | float   | define label's font outline size                                                                                                            |
> | position        | vector2 | define label's position                                                                                                                     |
> | size            | vector2 | define label's size (it will effects on "wrap_text" flag)                                                                                   |
> | pivot_offset    | vector2 | define label's pivot offset                                                                                                                 |
> | font_size       | int     | define label's font size                                                                                                                    |
> | y_separation    | float   | define label's y - separation                                                                                                               |
> | shaking         | float   | define label's shaking amount                                                                                                               |
> | z_order         | int     | define label's z-order                                                                                                                      |
> | wrap_text       | bool    | if it's true, label will automatically wrap text (see "wrap_mode"'s description below) else if it's false, label will clipped by it's size. |
> | visible         | bool    |                                                                                                                                             |
> | wrap_mode       | int     | (it will works when "wrap_text" flag is true) 0 means normal wrap mode, 1 means wrap by word mode                                           |

---

> audio
> 
> | `name`    | `type` | `description`                     |
> | --------- | ------ | --------------------------------- |
> | name      | string | define audio's name               |
> | file_path | string | define audio's file path          |
> | volume    | float  | define audio's volume (0.0 ~ 1.0) |

---

> history (or historycontainer)
> 
> | `name`     | `type`  | `description`                                                           |
> | ---------- | ------- | ----------------------------------------------------------------------- |
> | name       | string  | define historycontainer's name                                          |
> | position   | vector2 | define historycontainer's position                                      |
> | separation | float   | define historycontainer's y - separation between each history box       |
> | space      | float   | define historycontainer's x - separation between namebox and contentbox |
> | align_mode | int     | 0 = Begin, 1 = Center, 2 = End                                          |
> | visible    | bool    | define historycontainer's visible                                       |
> | z_order    | int     | define historycontainer's z-order                                       |
> | font_path  | string  | define historycontainer's font path                                     |
> | font_size  | int     | define historycontainer's font size                                     |
> | size       | vector2 | define historycontainer's size                                          |

# script

there's many usable keywords in `script.txt`.  
you can simply define your own label by using `label` keyword and call or jump by using `call` or `jump` keyword like Ren'Py.  
you can also use `if` keyword and `while` keyword like Python(there's no "for" keyword yet).  
you can replace for keyword to while). please check more information below.

### how do i create a character?

you can create character by using `char` keyword.

```python
# char [simplified name] [real name] [character's color]
char s "snipe" vec4(1, 0, 0, 1)
```

### how do i get my character to speak?

there's two syntax to get character to speak. it's same as Ren'Py's syntax.  
you should assert that you have been created dialog in the current scene.

```python
# [string]
"This is the text without character."
# [character] [string]
s "I am a snipe, the bird."
```

### how do i define a variable?

you can define variable by writting like this:
`[type] [name] [value]`
(you don't have to write an assign operator =)  
you can use comma(,) to define several variables at once, except for defining array.  
also, you can assign value with assign operator(=) on already defined variable.  
*you must write your variable name in pure english character or underbar, not a number*  

```python
# define a integer
int myInt 1, hello 2
# define a floating point
float myFloat 2.0, world 3.0
# define a string
string myString "Hello World"
# define a boolean
bool myBool true
# define an array (you cannot define several array at once)
array myArray 1, "2", true
# define a vector2
vector2 myVector2 vec2(100, 200)
# define a vector4
vector4 myVector4 vec4(1, 1, 1, 1)
# assign a value to variable
myInt = 2
hello = (3 + 3) * 10
myString = myString + "!"
myBool = not myBool
```

### how do i define a label and how to call it?

you can define a label with label keyword.
write label's content after the 4 spaces.
you don't have to write colon(:) like Ren'Py.
the two keyword, `call` and `jump` will work same as Ren'Py.

> you should use `call_main` or `jump_main` when your story starts.
> 
> because you should ignore whether it's callback label scope or global scope and call / jump to it as a global scope.

```python
# define a label
label myLabel
    s "This is myLabel."
    return
# call myLabel
call "myLabel"
# this message will be printed because you're using "call" keyword
print "can you hear me?"
```

```python
# define a label
label myLabel
    s "This is myLabel."
    return
# jump to myLabel
jump "myLabel"
# this message will not be printed because you're using "jump" keyword
print "can you hear me?"
```

```python
label main
    s "The main story starts with this message."
    return
# you should use "jump_main" instead of "jump"
label _on_start_button_pressed
    command "reset_all"
    jump_main "main"
```

### how do i use variables and labels which is already defined in another file?

you can use `include` keyword.

```python
include "res:\some_file.txt"
```

### how do i create if statement?

you can use `if` , `elif`, `else` keyword in script.txt. syntax is same as Python.  
(you don't have to write colon)
you can use these operator below:

- ==

- !=

- <=

- \>\=

- <

- \>

- not

- true

- false

- and

- or

```python
string myString "Hello World"
# it prints 2
if myString == "World Hello"
    print "1"
elif myString == "Hello World"
    print "2"
else
    print "3"
```

### how do i create while statement?

you can use `while` keyword to do that. (you don't have to write colon)
also, you can use break keyword(you cannot use continue keyword yet).

```python
int i 3
while i > 0
    print i
    i = i - 1
# it prints 3 2 1
```

or

```python
int i 3
while true
    print i
    i = i - 1
    if i < 1
        break
# it prints 3 2 1 too
```

### how do i make choice menu?

there's two ways to make it - use `choice` and `case` keyword.

```python
choice
    "What should I do?"
    case "eat cake"
        "You choose 1st choice."
    case "walk around"
        "You choose 2nd choice."
```

```python
string i ""
array choiceArr "eat cake" "walk around"
choice "i" "choiceArr"
    "What should I do?"
if i == "eat cake"
    "You choose 1st choice"
else
    "You choose 2nd choice."
```

### how do i save & load my game?

you should use `save` , `load`, `save_start` keyword.
here is the way how to use that keywords:

```python
label start
    # story index will initialize to 0 here.
    save_start
    s "The story starts here..."

label _on_start_button_pressed
    command "reset_all"
    jump_main "start"

label _on_load_button_pressed
    command "reset_all"
    # when you use "load" keyword, engine will start to
    # accelerate process until the story reach to
    # last saved position.
    load "res:\save.txt"
    jump_main "start"

label _on_save_button_pressed
    # when you use "save" keyword, engine will save
    # of story index, input & choice information.
    save "res:\save.txt"
```

### how do i save persistent data?

you can use `persistent` keyword.

`persistent [file path] "save" [var_name] [value]`

`persistent [file path] "load" [var_name]`

```python
# assume that this variable is registered as
# callback_value of "volume_slider"
float currentVolume 1.0
label _on_volume_slider_value_changed
    persistent "res:\persistent.txt" "save" "currentVolume" currentVolume
```

```python
persistent "res:\persistent.txt" "load" "currentVolume"
# now you will be able to use "currentVolume" variable.
```

### how do i print variable?

you can use `print` keyword:

```python
string myString "Hello World"
# it prints "Hello World!"
print myString + "!"
```

### how do i change scene?

you can use `scene` keyword:

```python
scene "nameOfScene"
```

### how do i use more utils?

you can use `command` keyword:

```python
command "set_visible" true "a" "b" ...
command "set_sprite_visible" true "name" "tag" ...
command "set_position" vec2(0, 0) "a" "b" ...
command "set_sprite_position" vec2(0, 0) "name" "tag"...
command "set_scale" vec2(1, 1) "a" "b" ...
command "set_sprite_scale" vec2(1, 1) "name" "tag" ...
command "set_rotation" 45.0 "a" "b" ...
command "set_sprite_rotation" 45.0 "name" "tag" ...
command "set_pivot_offset" vec2(5, 5) "a" "b" ...
command "set_sprite_pivot_offset" vec2(5, 5) "name" "tag"...
command "set_z_order" -1 "a" "b"
command "set_sprite_z_order" -1 "name" "tag"
command "set_camera_zoom" vec2(1.5, 1.5)
command "set_sprite_modulate" vec4(1, 0, 0, 1) "name" "tag"
command "set_color_rect_color" vec4(1, 0, 0, 1) "a" ...
command "open_url" "res:\README.html"
command "set_window_fullscreen" true
command "set_dialog_speed" 2.0
command "set_hslider_percent" "sliderName" 0.5
command "reset_dialog"
command "reset_choice"
command "reset_textedit"
command "reset_charmap"
command "reset_varmap"
command "reset_history"
command "reset_current_focused"
command "reset_all"
command "add_history" "name" "content"
command "pause_dialog"
```

### how do i animate object?

you can use `tween` keyword:

```python
tween "sprite_position" "name" "tag" 5.0 vec2(0, 0) vec2(100, 100)
```

or

```python
tween await "sprite_position" "name" "tag" 5.0 vec2(0, 0) vec2(100, 100)
```

if you add "await" keyword after tween keyword,  engine will await until the tween finishes.  
you can use these string as a 2nd parameter:

- position

- position:x

- position:y

- sprite_position

- sprite_position:x

- sprite_position:y

- sprite_scale

- color_rect_color

- sprite_modulate

### how do i show / hide image?

you can use `show` and `hide` keyword.
also, you can add transition with `with` keyword.
you can use 3 transition:

- fade - fade in / fade out of sprite's modulate.

- dissolve - hide all images with the same name except for the given tag.
  (show method only)

- move - show / hide with movement.

```python
show "spriteName" "spriteTag"
hide "spriteName" "spriteTag"
```

```python
show "spriteName" "spriteTag"
    with "dissolve" 0.5
    with "move" 0.5 vec2(0, 0) vec2(100, 0)
# with "fade" [duration]
# with "dissolve" [duration]
# with "move" [duration] [start_pos] [end_pos]
hide "spriteName" "spriteTag"
    with await "fade" 0.5
```

### how do i wait for given seconds?

use `await` keyword.

```python
# engine will wait for 1 seconds
await 1.0
```

### how do i get text input from player?

you can use `input` keyword to do that.  
you have to create textedit in current scene already.

```python
#input "nameOfTextEdit" "targetVariableName" maxCharacter
string player_name ""
input "myTextEdit" "player_name" 10

# now you can create character with that value
char p player_name vec4(0, 0, 0, 1)
```

### how do i quit game?

use `quit` keyword.

```python
label _on_quit_button_pressed
    quit
```

### how do i control audio?

you can use `audio` keyword to play / stop audio file and change volume of it.

```python
audio "play" "myAudio"
audio "play_loop" "myAudio"
audio "play_fade" "myAudio" 1.0 # duration
audio "play_loop_fade" "myAudio" 1.0 # duration
audio "stop" "myAudio"
audio "stop_fade" "myAudio" 1.0 # duration
audio "set_volume" "myAudio" 0.5 # value(0.0 ~ 1.0)
```

### is there built-in functions that can be used in script?

all of functions that can be used are associated with array type.

- remove(array, variant) -> array

- empty(array) -> bool

- len(array) -> int

- insert(array, at,  variant) -> array

- append(array, variant)

- count(array, variant) -> int

- sort(array) -> array

- delete(array, at) -> array

here is the way how to use it.

```python
array myArray 1, 2, 3, 4, 5, "Hello", "World"
# remove "hello" from array.
myArray = remove(myArray, "Hello")

# it prints "false"
if empty(myArray) == true
    print "true"
else
    print "false"

# it prints 6
print len(myArray)

# now array back to original
myArray = insert(myArray, 5, "Hello")

# append "i love you" to myArray
myArray = append(myArray, "i love you")

# it prints 1
print count(myArray, 3)

myArray = [3, 5, 2, 1, 4]
# sort by greater value. now myArray is [1, 2, 3, 4, 5]
myArray = sort(myArray)

# delete element with index. now myArray is [1, 2, 3, 5]
myArray = delete(myArray, 3)
```
