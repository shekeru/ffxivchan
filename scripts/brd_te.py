# Dogshit
import string, time
script = '''
C Eb F Eb F F F Bb G# G F G
G Bb +C F Eb Bb Bb G Bb Bb +C

Eb -Bb -Bb Eb Eb F -Bb -Bb
-Bb G G# G F Eb F G G# G C
C D Eb Eb D D
Eb F G# G F Eb
G G F E F C C D D
Eb -Bb -Bb Eb Eb F -Bb -Bb
-Bb G G# G F Eb F G G# G C
C D Eb Eb D D
Eb F G# G F Eb
G G F E F G G# G
G A B

Eb Eb D Eb Eb D F F Eb D C D Eb Eb D F D C F G G# Bb
Eb Eb D Eb Eb D F F Eb D Eb F G G# G F Eb F G
G A B

C Eb F Eb F F F Bb G# G F G
G Bb +C F Eb D D C D F Eb Eb
C Eb F Eb F F F Bb G# G F G
G Bb +C F Eb Bb Bb G Bb Bb +C
C Eb F Eb F F F Bb G# G F G
G Bb +C F Eb Bb Bb G Bb Bb +C
'''.strip().split()
# Shit autos
from pywinauto import Application
app = Application().connect(process=394944)
def execute(k):
    app.FFXIVGAME.type_keys(k)
    time.sleep(.25)
for note in script:
    if len(note) == 1:
        execute(note)
