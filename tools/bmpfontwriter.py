import sys, pygame, re, PIL.Image as Image, PIL.ImageDraw as ImageDraw

DEBUG = 1

grid_line_width = 2

screen_size = (768,512)

font_size = 0

ofilename = ''
ifilename = ''

def prompt_for(msg, regexp):
    while(1):
        sys.stdout.write(msg)
        trialline = sys.stdin.readline().translate(None, '\r\n')
        r = re.compile(regexp)
        if(r.match(trialline) == None):
            sys.stderr.write("Incorrect format! Filename has to match " + "\'" + regexp + "\'\n")
            continue

        return trialline

option = 0

while(1):
    option = int(prompt_for("Select one:\n\tOpen a file: 0\n\tMake a new font: 1\n: ", '^[0-9]+$'))
    if(option == 0 or option == 1):
        break
    sys.stderr.write("Invalid option!\n")

inimage = 0

if(option == 0):
    while(1):
        ifilename = prompt_for("Input filename to load from (*.png): ", '^(\.\/[0-9A-Za-z\-_\/]*\/)?[0-9A-Za-z\-_]+\.png$')
        try:
            inimage = Image.open(ifilename)
        except Exception:
            sys.stderr.write("Invalid filename!\n")
            continue

        if(not (inimage.size[0] % 256 == 0) or (inimage.size[0] == 0 or inimage.size[1] == 0)):
            sys.stderr.write("Image is of incorrect size.\n"+
                             "Width must be a multiple of 256,\n"+
                             "and dimensions must be nonzero.\n")
            continue

        font_size = (inimage.size[0]/256, inimage.size[1])
        
        break   

if(not option == 0):
    while(1):
        sys.stdout.write("Input size of font (width, height): ")
        r = re.compile('\([ ]*([0-9]+)[ ]*,[ ]*([0-9]+)[ ]*\)')
        matchstrs = r.split(sys.stdin.readline())
        if(not len(matchstrs) == 4):
            sys.stderr.write("Incorrect format! (Example: \"(5, 7)\")\n")
            continue

        font_size = tuple([int(x) for x in matchstrs[1:3]])
        break

ofilename = prompt_for("Input filename to use when done (*.png): ", '^[0-9A-Za-z\-_]+\.png$')

#print font_size

pygame.init()

prompt_font = pygame.font.SysFont("Lucida Console", 24, False, False)
big_prompt_font = pygame.font.SysFont("Lucida Console", 256, False, False)

black = (0,0,0)
pixcolor = (20,20,20)
white = (255,255,255)

screen = pygame.display.set_mode(screen_size)
pygame.display.set_caption("Bitmap Font Writer v0.1")

pixfield_ulhpos = (20,20)
pixfield_cell_size = int(min((
    (screen_size[0]/2 - pixfield_ulhpos[0]*2)/font_size[0],
    (screen_size[1] - pixfield_ulhpos[1]*2)/font_size[1]
    )))

pixfield_size = font_size[:]

def init_data(size, val):
    return [[val for x in range(size[1])] for y in range(size[0])]

pixfield_data = init_data(pixfield_size, False)

finished_data = [None for i in range(256)]

## buttons and control
saveChar = False
current_character = (ord(' ')+1)

buttons_list = []

def char_forward():
    global pixfield_data
    global saveChar
    global current_character
    global finished_data
    # store the pixfield data array into the finished data array along with the edit status
    finished_data[current_character] = (pixfield_data, saveChar)
    current_character += 1
    current_character %= 255

    # if the new character has no data in the finished data array
    if(finished_data[current_character] == None):
        # initialize it and initialize pixfield_data
        pixfield_data = init_data(pixfield_size, False)
        saveChar = False
        finished_data[current_character] = (pixfield_data, saveChar)
    else:
        pixfield_data, saveChar = finished_data[current_character]

def char_back():
    global pixfield_data
    global saveChar
    global current_character
    global finished_data
    # store the pixfield data array into the finished data array along with the edit status
    finished_data[current_character] = (pixfield_data, saveChar)
    current_character -= 1
    current_character %= 255

    # if the new character has no data in the finished data array
    if(finished_data[current_character] == None):
        # initialize it and initialize pixfield_data
        pixfield_data = init_data(pixfield_size, False)
        saveChar = False
        finished_data[current_character] = (pixfield_data, saveChar)
    else:
        pixfield_data, saveChar = finished_data[current_character]

def pixfield_pos(cell_size, pos, grid_size, xy):
    return (pos[0] + cell_size*xy[0], pos[1] + cell_size*xy[1])

def pixfield_pos_rl(cell_size, pos, grid_size, xyscreen):
    ulhc = pixfield_pos(cell_size, pos, grid_size, (0,0))
    targpos = ((xyscreen[0]-ulhc[0])/cell_size, (xyscreen[1]-ulhc[1])/cell_size)

    if(targpos[0] < 0 or
       targpos[0] >= grid_size[0] or
       targpos[1] < 0 or
       targpos[1] >= grid_size[1]):
        return None

    return targpos

def pixfield_draw(cell_size, pos, grid_size, data):
    if(grid_size[0] == 0 or grid_size[1] == 0):
        return
    
    pygame.draw.rect(screen, black, (pos[0],
                                     pos[1],
                                     pixfield_size[0]*pixfield_cell_size+1,
                                     pixfield_size[1]*pixfield_cell_size+1), grid_line_width)

    for i in range(0, grid_size[0]):
        for j in range(0, grid_size[1]):
            ulh = pixfield_pos(cell_size, pos, grid_size, (i, j))
            if data[i][j]:
                pygame.draw.rect(screen, pixcolor,
                                 (ulh[0]+grid_line_width,
                                  ulh[1]+grid_line_width,
                                  cell_size-grid_line_width,
                                  cell_size-grid_line_width))
                             
    
    for i in range(1, grid_size[0]):
        pygame.draw.line(screen, black,
                         pixfield_pos(cell_size, pos, grid_size, (i, 0)),
                         pixfield_pos(cell_size, pos, grid_size, (i, grid_size[1])), grid_line_width)
    for j in range(1, grid_size[1]):
        pygame.draw.line(screen, black,
                         pixfield_pos(cell_size, pos, grid_size, (0, j)),
                         pixfield_pos(cell_size, pos, grid_size, (grid_size[0], j)), grid_line_width)

def check_buttons(mousepos, callback=True):
    hitbuttons = []
    for button in buttons_list:
        if(pygame.Rect(*button[0]).collidepoint(mousepos)):
            hitbuttons.append(button)
            button[2]()
    return hitbuttons
            

def process_mouseup(mousepos):
    check_buttons(mousepos)

def finish_up():
    global pixfield_data
    global saveChar
    global current_character
    global finished_data
    global ofilename
    global pixfield_size
    # store the pixfield data array into the finished data array along with the edit status
    finished_data[current_character] = (pixfield_data, saveChar)

    img = Image.new('RGB', (256*pixfield_size[0], pixfield_size[1]))

    draw = ImageDraw.Draw(img)
    draw.rectangle([(0,0), (256*pixfield_size[0], pixfield_size[1])], white)
    del draw
                   
    exportpix = img.load()

    xcursor = -pixfield_size[0]
    
    for data_element in finished_data:
        
        xcursor += pixfield_size[0]
        if(data_element == None):
            continue
        (pixdata, savestatus) = data_element
        if(not savestatus):
            continue

        pixelx = -1
        for col in pixdata:
            pixelx += 1
            for pixely in range(pixfield_size[1]):
                if(col[pixely]):
                    exportpix[xcursor + pixelx, pixely] = black
                else:
                    exportpix[xcursor + pixelx, pixely] = white
                    

    img.save(ofilename, 'PNG')
    pygame.quit()
    sys.exit()

def load_image():
    global pixfield_data
    global finished_data
    global ofilename
    global pixfield_size
    global inimage
    global current_character
    global saveChar
    # store the pixfield data array into the finished data array along with the edit status
    finished_data = [(init_data(pixfield_size, False), True) for i in range(256)]

    current_character = ord(' ')+1

    importpix = inimage.load()

    xcursor = -pixfield_size[0]
    
    for data_element in finished_data:
        
        xcursor += pixfield_size[0]
        if(data_element == None):
            continue
        (pixdata, savestatus) = data_element
        if(not savestatus):
            continue

        pixelx = -1
        for col in pixdata:
            pixelx += 1
            for pixely in range(pixfield_size[1]):
                if(importpix[xcursor + pixelx, pixely][0] > 127):
                    col[pixely] = False
                else:
                    col[pixely] = True

    pixfield_data, saveChar = finished_data[current_character]

def char_clear():
    global pixfield_data

    for i in range(len(pixfield_data)):
        for j in range(len(pixfield_data[i])):
            pixfield_data[i][j] = False

draggingflag = False

backbutton = ((screen_size[0]/2 + 20,
               screen_size[1]-70,100,50),
              "back",
              char_back)
nextbutton = ((screen_size[0]/2 + 20+120,
               screen_size[1]-70,100,50),
              "next",
              char_forward)
donebutton = ((screen_size[0]/2 + 20+240,
               screen_size[1]-70,100,50),
              "done",
              finish_up)
clearbutton = ((screen_size[0]/2 + 20+240,
               screen_size[1]-140,100,50),
              "clear",
              char_clear)
buttons_list.append(backbutton)
buttons_list.append(nextbutton)
buttons_list.append(donebutton)
buttons_list.append(clearbutton)

def text_in_box(box, text, font, color):
    textsurf = font.render(text, False, color)
    destRect = pygame.Rect((0,0), textsurf.get_size())
    destRect.center = pygame.Rect(*box).center
    pygame.draw.rect(screen, color, box, grid_line_width)
    screen.blit(textsurf, destRect.topleft)

def draw_buttons():
    for button in buttons_list:
        text_in_box(button[0], button[1], prompt_font, black)

def get_disp_char(n):
    if(n < 0 or n > 255):
        return "INV"
    if(n <= ord(' ')):
        return [
            'NUL',
            'SOH',
            'STX',
            'ETX',
            'EOT',
            'ENQ',
            'ACK',
            'BEL',
            'BS',
            'TAB',
            'LF',
            'VT',
            'FF',
            'CR',
            'SO',
            'SI',
            'DLE',
            'DC1',
            'DC2',
            'DC3',
            'DC4',
            'NAK',
            'SYN',
            'ETB',
            'CAN',
            'EM',
            'SUB',
            'ESC',
            'FS',
            'GS',
            'RS',
            'US',
            'SPC'
            ][n]
    return chr(n)

if(option == 0):
    load_image()

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
##        if event.type == pygame.KEYUP and event.key == pygame.K_q:
##            pygame.quit()
##            sys.exit()
        if event.type == pygame.KEYUP:
            if event.key == pygame.K_RIGHT:
                char_forward()
            if event.key == pygame.K_LEFT:
                char_back()
        if event.type == pygame.MOUSEMOTION:
            mousepos = event.pos
            buttons = event.buttons

            gridmpos = pixfield_pos_rl(pixfield_cell_size, pixfield_ulhpos,
                                       pixfield_size, mousepos)

            if(not gridmpos == None):
                if(buttons[0] == 1):
                    pixfield_data[gridmpos[0]][gridmpos[1]] = True
                    draggingflag = True
                    saveChar = True
                if(buttons[2] == 1):
                    pixfield_data[gridmpos[0]][gridmpos[1]] = False
                    draggingflag = True
                    saveChar = True
                    
        if event.type == pygame.MOUSEBUTTONUP:
            mousepos = event.pos
            button = event.button

            gridmpos = pixfield_pos_rl(pixfield_cell_size, pixfield_ulhpos,
                                       pixfield_size, mousepos)

            if(not gridmpos == None and button == 1 and not draggingflag):
                pixfield_data[gridmpos[0]][gridmpos[1]] = not pixfield_data[gridmpos[0]][gridmpos[1]]
                saveChar = True
            if(not gridmpos == None and button == 3 and not draggingflag):
                pixfield_data[gridmpos[0]][gridmpos[1]] = False
                saveChar = True

            draggingflag = False

            process_mouseup(mousepos)
    
    screen.fill(white)

    # draw the pixel field
    pixfield_draw(pixfield_cell_size, pixfield_ulhpos,
                  pixfield_size, pixfield_data)

    # draw the letter currently being drawn
    textsurf = prompt_font.render("Character code %d:" % current_character, True, black)

    destrect = screen.blit(textsurf, (screen_size[0]/2 + 20, 20))

    textsurf = big_prompt_font.render(get_disp_char(current_character), True, black)

    # draw the buttons
    draw_buttons()

    screen.blit(textsurf, destrect.bottomleft)
    
    pygame.display.flip()
