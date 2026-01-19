defmodule Fishing.Registration.Competitors do
  use Ecto.Schema
  import Ecto.Changeset

  schema "competitor" do
    field :first_name, :string
    field :last_name, :string
    field :email, :string

    timestamps()
  end

  @doc false
  def changeset(competitors, attrs) do
    competitors
    |> cast(attrs, [:first_name, :last_name, :email])
    |> validate_required([:first_name, :last_name, :email])
  end
end
