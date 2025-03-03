#ifndef BOOK_EVENT_HANDLER_HEADER
#define BOOK_EVENT_HANDLER_HEADER

#define AME_TOK_BOOK 0x41424B54

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "REL/Relocation.h"
#include "SKSE/Logger.h"
#include "RE/B/BSTEvent.h"
#include "RE/T/TESContainerChangedEvent.h"
#include <vector>

class BookEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent>
{
public:
    static std::vector<RE::FormID> acquiredBooks;
    static BookEventHandler* GetSingleton();
    static void Register();

    RE::BSEventNotifyControl ProcessEvent(
        const RE::TESContainerChangedEvent* event,
        RE::BSTEventSource<RE::TESContainerChangedEvent>* source
    ) override;

private:
    BookEventHandler() = default;
    ~BookEventHandler() = default;
    
    static void RegisterSerialization();
    static void SaveCallback(SKSE::SerializationInterface* serialization);
    static void LoadCallback(SKSE::SerializationInterface* serialization);
};

#endif // !BOOK_EVENT_HANDLER_HEADER
