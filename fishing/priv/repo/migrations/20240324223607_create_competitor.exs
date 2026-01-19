defmodule Fishing.Repo.Migrations.CreateCompetitor do
  use Ecto.Migration

  def change do
    create table(:competitor) do
      add :first_name, :string, null: false
      add :last_name, :string, null: false
      add :email, :string, null: false

      timestamps()
    end
  end
end
