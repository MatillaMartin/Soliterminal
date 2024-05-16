#include "AppRender.h"

namespace panda
{
	AppRender::AppRender(const App& app, Renders renders)
		: m_app(app)
		, m_renders(std::move(renders))
	{
	}

	void AppRender::update()
	{
		if (m_app.state() == App::State::Game)
			m_renders.gameRender.update();
		else if (m_app.state() == App::State::Pause)
			m_renders.menuRender.update();
	}
}