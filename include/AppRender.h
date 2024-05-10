#pragma once
#include "App.h"
#include "AppControl.h"
#include "GameRender.h"
#include "MenuRender.h"
#include "Render.h"

namespace panda
{
	class AppRender : public Render
	{
	public:
		struct Renders
		{
			GameRender& gameRender;
			MenuRender& menuRender;
		};

		AppRender(const App& app, Renders renders)
			: m_app(app)
			, m_renders(std::move(renders))
		{
		}

		void update() override
		{
			if (m_app.state() == App::State::Game)
				m_renders.gameRender.update();
			else if (m_app.state() == App::State::Pause)
				m_renders.menuRender.update();
		}

	private:
		const App& m_app;
		Renders m_renders;
	};
}