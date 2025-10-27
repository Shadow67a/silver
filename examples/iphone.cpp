#include <iostream>
#include <vector>
#include <cmath>

#include "../include/silver.hpp"

silver::Font dateFont;
silver::Font customizeFont;
silver::Font nyHeavy;
silver::Font rails;
silver::Font rounded;
silver::Font stencil;

struct contentView : silver::View
{
	silver::Image *i;
	silver::Text *clock;
	silver::Text *date;

	silver::Quad *q;

	silver::Text *customizeText;

	silver::Text *nyHeavyT;
	silver::Quad *nyHeavyQ;
	silver::State q1{0.0};
        silver::Text *railsT;
	silver::Quad *railsQ;
	silver::State q2{0.0};
	silver::Text *stencilT;
	silver::Quad *stencilQ;
	silver::State q3{0.0};

	silver::Quad *separator;
	silver::State s{-0.55};

	contentView()
	{
		dateFont = silver::Font("./assets/semiboldsoft.otf");
		customizeFont = silver::Font("./assets/bold.otf");
		nyHeavy = silver::Font("./assets/nyheavy.otf");
		rails = silver::Font("./assets/rails.otf");
		rounded = silver::Font("./assets/rounded.otf");
		stencil = silver::Font("./assets/stencil.otf");

		background(silver::hex(0xffffff));
		i = new silver::Image("./assets/bg.jpg");
		i->scale(0.38, 0.85);

		clock = new silver::Text("7:02");
		clock->font(rails)
		 ->fill(silver::hex(0xfdf0ff))
		 ->scale(116.0)
		 ->position(0.0, 0.65, 0.0);

		date = new silver::Text("Monday\u2019 September 30");
		date->font(dateFont)
		 ->fill(silver::hex(0xfdf0ff))
		 ->scale(25)
		 ->position(0.0, 0.82, -0.1);

		q = new silver::Quad();
		q->fill(silver::hex(0xfffcff))
		 ->scale(0.38, 0.40)
		 ->position(0.0, animate(1.0, easeOutExpo, s), 0.0)
		 ->cornerRadius(0.1)
		 ->onHover([this]{
			s = -0.3;
		}, [this]{
			s = -0.55;
		});

		customizeText = new silver::Text("Font & Colour");
		customizeText->font(customizeFont)
		->scale(24)
		->position(0.0, 0.41, 0.0)
		->parent(q);

		nyHeavyT = new silver::Text("12");
		nyHeavyT->font(nyHeavy)
		->scale(114)
		->position(-0.3, 0.185, 0.0)
		->parent(q);

		nyHeavyQ = new silver::Quad();
		nyHeavyQ->fill(silver::hex(0xffffff))
		->border(silver::hex(0x3464eb), animate(0.5, easeOutExpo, q1))
		->cornerRadius(0.3)
		->scale(0.11, 0.11)
		->position(0.125, 0.075, 0.0)
		->parent(q)
		->onClick([this]{
			q1 = 8.0;
			q2 = 0.0;
			q3 = 0.0;
			clock->font(nyHeavy);
		});

		railsT = new silver::Text("12");
                railsT->font(rails)
                ->scale(114)
                ->position(0.0, 0.185, 0.0)
                ->parent(q);

		railsQ = new silver::Quad();
                railsQ->fill(silver::hex(0xffffff))
                ->border(silver::hex(0x3464eb), animate(0.5, easeOutExpo, q2))
                ->cornerRadius(0.3)
                ->scale(0.11, 0.11)
                ->position(0.0, 0.075, 0.0)
                ->parent(q)
                ->onClick([this]{
                        q1 = 0.0;
                        q2 = 8.0;
                        q3 = 0.0;
			clock->font(rails);
                });

		stencilT = new silver::Text("12");
                stencilT->font(stencil)
                ->scale(114)
                ->position(0.3, 0.185, 0.0)
                ->parent(q);

		stencilQ = new silver::Quad();
                stencilQ->fill(silver::hex(0xffffff))
                ->border(silver::hex(0x3464eb), animate(0.5, easeOutExpo, q3))
                ->cornerRadius(0.3)
                ->scale(0.11, 0.11)
                ->position(-0.125, 0.075, 0.0)
                ->parent(q)
                ->onClick([this]{
                        q1 = 0.0;
                        q2 = 0.0;
                        q3 = 8.0;
			clock->font(stencil);
                });

		separator = new silver::Quad();
		separator->fill(silver::hex(0xd9d9d9))
			 ->position(0.0, 0.0, 0.0)
			 ->scale(0.38, 0.004)
			 ->cornerRadius(0.01)
			 ->parent(q);
	};

	std::vector<silver::View*> body() override
	{
		std::vector<silver::View*> e = {
			nyHeavyQ, railsQ, stencilQ, separator, q, i, clock, date, customizeText, nyHeavyT, railsT, stencilT
		};

		return e;
	};
};

int main()
{
	silver::App app(468, 1013, "iPhone");
	app.mainView(new contentView())
	   .run();
	return 0;
};

