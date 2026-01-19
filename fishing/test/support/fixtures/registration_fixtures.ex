defmodule Fishing.RegistrationFixtures do
  @moduledoc """
  This module defines test helpers for creating
  entities via the `Fishing.Registration` context.
  """

  @doc """
  Generate a competitors.
  """
  def competitors_fixture(attrs \\ %{}) do
    {:ok, competitors} =
      attrs
      |> Enum.into(%{
        email: "some email",
        first_name: "some first_name",
        last_name: "some last_name"
      })
      |> Fishing.Registration.create_competitors()

    competitors
  end
end
