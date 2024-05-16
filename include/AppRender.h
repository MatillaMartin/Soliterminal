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

		AppRender(const App& app, Renders renders);
		void update() override;

	private:
		const App& m_app;
		Renders m_renders;
	};
}