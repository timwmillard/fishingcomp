defmodule Fishing.Registration do
  @moduledoc """
  The Registration context.
  """

  import Ecto.Query, warn: false
  alias Fishing.Repo

  alias Fishing.Registration.Competitors

  @doc """
  Returns the list of competitor.

  ## Examples

      iex> list_competitor()
      [%Competitors{}, ...]

  """
  def list_competitor do
    Repo.all(Competitors)
  end

  @doc """
  Gets a single competitors.

  Raises `Ecto.NoResultsError` if the Competitors does not exist.

  ## Examples

      iex> get_competitors!(123)
      %Competitors{}

      iex> get_competitors!(456)
      ** (Ecto.NoResultsError)

  """
  def get_competitors!(id), do: Repo.get!(Competitors, id)

  @doc """
  Creates a competitors.

  ## Examples

      iex> create_competitors(%{field: value})
      {:ok, %Competitors{}}

      iex> create_competitors(%{field: bad_value})
      {:error, %Ecto.Changeset{}}

  """
  def create_competitors(attrs \\ %{}) do
    %Competitors{}
    |> Competitors.changeset(attrs)
    |> Repo.insert()
  end

  @doc """
  Updates a competitors.

  ## Examples

      iex> update_competitors(competitors, %{field: new_value})
      {:ok, %Competitors{}}

      iex> update_competitors(competitors, %{field: bad_value})
      {:error, %Ecto.Changeset{}}

  """
  def update_competitors(%Competitors{} = competitors, attrs) do
    competitors
    |> Competitors.changeset(attrs)
    |> Repo.update()
  end

  @doc """
  Deletes a competitors.

  ## Examples

      iex> delete_competitors(competitors)
      {:ok, %Competitors{}}

      iex> delete_competitors(competitors)
      {:error, %Ecto.Changeset{}}

  """
  def delete_competitors(%Competitors{} = competitors) do
    Repo.delete(competitors)
  end

  @doc """
  Returns an `%Ecto.Changeset{}` for tracking competitors changes.

  ## Examples

      iex> change_competitors(competitors)
      %Ecto.Changeset{data: %Competitors{}}

  """
  def change_competitors(%Competitors{} = competitors, attrs \\ %{}) do
    Competitors.changeset(competitors, attrs)
  end
end
