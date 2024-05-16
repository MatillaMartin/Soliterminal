#pragma once
#include "Action.h"
#include "App.h"
#include "GameControl.h"
#include "MenuControl.h"

namespace panda
{
	class AppControl : public ActionListener
	{
	public:
		struct Controls
		{
			GameControl& gameControl;
			MenuControl& menuControl;
		};

		AppControl(App& app, Controls controls);
		void action(const Action& action);

	private:
		App& m_app;
		Controls m_controls;
	};
}