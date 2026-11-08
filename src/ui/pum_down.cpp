// ShutdownSubtitles empties the subtitle queue and rebuilds it in place. Part of the original pumhandler.cpp.
#include "SubtitleQueue.h"

void ShutdownSubtitles() {
    dwi::list<QueuedSubtitle> *queue = &g_subtitle_queue;
    queue->~list();
    if (queue)
        new (queue) dwi::list<QueuedSubtitle>;
    g_subtitle_queue_initialized = 0;
}
