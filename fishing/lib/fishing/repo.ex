defmodule Fishing.Repo do
  use Ecto.Repo,
    otp_app: :fishing,
    adapter: Ecto.Adapters.Postgres
end
