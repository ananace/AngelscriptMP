LD34
=====

My Ludum Dare 34 entry


Dependencies
============

- CMake
- SFML 3.2
- Angelscript 2.30

Example Script
---------------

```angelscript
void OnLoad()
{
	Clock t;
}

class Clock
{
	Clock()
	{
		Hooks::Add("DrawUI", "draw");

		font = Resources::GetFont("arial.ttf");
		target = Time::At(2015,12,12, 03,00,00);
	}
	~Clock()
	{
		Hooks::Remove("DrawUI");
	}

	string toString(const Timespan&in span)
	{
		float sec = span.Seconds;

		int hours   = int(sec / 3600);
		int minutes = int(sec % 3600) / 60;
		int seconds = int(sec % 3600) % 60;

		return (hours <= 9 ? "0" : "") + hours + ":" +
			(minutes <= 9 ? "0" : "") + minutes + ":" +
			(seconds <= 9 ? "0" : "") + seconds;
	}

	void draw(sf::Renderer@ rend)
	{
		Timespan remain = (target - Time::Now);

		sf::Text title("Time remaining until theme:\n");
		title.SetFont(@font.Font);

		title.CharacterSize = 20;
		title.Origin = title.LocalBounds.Center;
		title.Position = rend.Center - sf::Vec2(0, 250);

		rend.Draw(title);
		title.String = toString(remain);
		title.Origin = title.LocalBounds.Center;

		title.Move(sf::Vec2(0, 18));

		rend.Draw(title);

		{
			sf::RectangleShape dial(sf::Vec2(5, 100));

			float timeleft = remain.Seconds;

			dial.Origin = sf::Vec2(2.5, 100);
			dial.Position = rend.Center;

			dial.Rotation = (timeleft / 3600) * (360 / 12);

			rend.Draw(dial);

			dial.Scale = sf::Vec2(0.75, 1.5);

			dial.Rotation = ((timeleft % 3600) / 60) * (360 / 60);

			rend.Draw(dial);

			dial.Scale = sf::Vec2(0.5, 1.75);
			dial.FillColor = sf::Color::Red;

			dial.Rotation = ((timeleft % 3600) % 60) * (360 / 60);

			rend.Draw(dial);
		}

		sf::RectangleShape face(sf::Vec2(4, 25));
		face.Origin = sf::Vec2(2, 25);

		const int j = 24;
		for (int i = 0; i < j; ++i)
		{
			if (i % 2 == 1)
				face.Scale = sf::Vec2(0.25, 1);
			else
				face.Scale = sf::Vec2(1, 1);

			float ang = i * (360 / j);

			face.Rotation = 90 + ang;
			face.Position = rend.Center +
				sf::Vec2(
					cos(ang * (3.1415/180)) * 150,
					sin(ang * (3.141519/180)) * 150
				);

			rend.Draw(face);
		}
	}

	private Resources::Font font;
	private Timestamp nextReport;
	private Timestamp target;
}

```