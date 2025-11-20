<<<<<<< HEAD
﻿#include "button.h"
=======
#include "button.h"
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
#include "raylib.h"

void DrawButton(const Button& b) {
    DrawRectangleRec(b.rect, b.color);
    DrawRectangleLines((int)b.rect.x, (int)b.rect.y, (int)b.rect.width, (int)b.rect.height, Fade(BLACK, 0.2f));
    DrawText(b.text,
        (int)(b.rect.x + (b.rect.width - MeasureText(b.text, 20)) / 2),
        (int)(b.rect.y + (b.rect.height - 20) / 2),
        20, WHITE);
<<<<<<< HEAD
}
=======
}
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
