#include "AppControl.h"

#include "Action.h"

namespace panda
{
	AppControl::AppControl(App& app, Controls controls)
		: m_app(app)
		, m_controls(std::move(controls))
	{
	}

	void AppControl::action(const Action& action)
	{
		if (action == Action::Exit)
		{
			App::State nextState = App::State::Pause;
			if (m_app.state() == App::State::Pause)
				nextState = App::State::Game;

			m_app.setState(nextState);
		}


		if (m_app.state() == App::State::Game)
			m_controls.gameControl.action(action);

		else if (m_app.state() == App::State::Pause)
			m_controls.menuControl.action(action);
	}
}