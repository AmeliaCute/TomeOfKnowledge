#include "BookEventHandler.hpp"

std::vector<RE::FormID> BookEventHandler::acquiredBooks;

BookEventHandler* BookEventHandler::GetSingleton()
{
    static BookEventHandler singleton;
    return &singleton;
}

void BookEventHandler::Register()
{
    RegisterSerialization(); // dont have the choice 
    auto events = RE::ScriptEventSourceHolder::GetSingleton();
    if(events)
    {
        events->AddEventSink(GetSingleton());
        SKSE::log::info("Book EventHandler registered.");
    }
}

RE::BSEventNotifyControl BookEventHandler::ProcessEvent(
    const RE::TESContainerChangedEvent* event,
    RE::BSTEventSource<RE::TESContainerChangedEvent>*
){
    if(!event || !event->newContainer)
        return RE::BSEventNotifyControl::kContinue;

    auto* player = RE::PlayerCharacter::GetSingleton();
    if(!player)

        return RE::BSEventNotifyControl::kContinue;

    if(event->newContainer != player->GetFormID())
        return RE::BSEventNotifyControl::kContinue;

    auto* form = RE::TESForm::LookupByID<RE::TESObjectBOOK>(event->baseObj);
    if(form)
    {

        acquiredBooks.push_back(form->formID);
        SKSE::log::info("Acquired book {}", form->fullName);
    }

    return RE::BSEventNotifyControl::kContinue;
}

void BookEventHandler::SaveCallback(SKSE::SerializationInterface* serialization)
{
    if(serialization->OpenRecord(AME_TOK_BOOK, 0))
    {
        std::uint32_t count = static_cast<std::uint32_t>(acquiredBooks.size());
        serialization->WriteRecordData(&count, sizeof(count));

        for(const auto& book : acquiredBooks)
            serialization->WriteRecordData(&book, sizeof(book));

        SKSE::log::info("Book saved successfully with size of {}", count);
    }
}

void BookEventHandler::LoadCallback(SKSE::SerializationInterface* serialization)
{
    acquiredBooks.clear();

    std::uint32_t type = 0, version = 0, length = 0;

    while(serialization->GetNextRecordInfo(type, version, length))
    {
        if(type == AME_TOK_BOOK)
        {
            std::uint32_t count = 0;
            if(!serialization->ReadRecordData(&count, sizeof(count)))
            {
                SKSE::log::error("Failed to read count of acquired books.");
                return;
            }

            for(std::uint32_t i = 0; i < count; ++i)
            {
                RE::FormID formID = 0;
                if(!serialization->ReadRecordData(&formID, sizeof(formID)))
                {
                    SKSE::log::error("Failed to read book formID at index {}.", i);
                    return;
                }
                acquiredBooks.push_back(formID);
            }
            SKSE::log::info("Book loaded successfully with size of {}", count);
        }
    }
}

void BookEventHandler::RegisterSerialization()
{
    SKSE::GetSerializationInterface()->SetUniqueID(AME_TOK_BOOK);
    SKSE::GetSerializationInterface()->SetSaveCallback(SaveCallback);
    SKSE::GetSerializationInterface()->SetLoadCallback(LoadCallback);
}