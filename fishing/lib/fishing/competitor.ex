defmodule Fishing.Competitor do
  use Ecto.Schema
  import Ecto.Changeset

  @schema_prefix "fishing"

  schema "competitor" do

    field :competitor_no, :string
    field :first_name, :string
    field :last_name, :string
    field :email, :string
    field :mobile, :string
    field :event_id, :integer

  end

  @doc false
  def changeset(competitor, attrs) do
    competitor
    |> cast(attrs, [:competitor_no, :first_name, :last_name, :email, :mobile])
    |> validate_required([:competitor_no, :first_name, :last_name])
    # |> validate_number(:competitor_no, greater_than: 0)
  end
end
