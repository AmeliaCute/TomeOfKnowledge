#include "GetBook.hpp"

bool GetBook_CMD(RE::StaticFunctionTag*, RE::TESObjectREFR* a_this) 
{
	if (a_this)
	{
		RE::ConsoleLog* console = RE::ConsoleLog::GetSingleton();
        if (!console) return false;

		auto* form = RE::TESForm::LookupByID<RE::TESObjectBOOK>(a_this->formID);
        if (!form)
            console->Print("Exist but is not a book.");
        else
            console->Print(std::format("Book name: {}", form->GetName()).c_str());
	}
	else
	RE::DebugNotification("Wtf aru doing??");

	return true;
}
