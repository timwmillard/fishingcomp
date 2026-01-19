defmodule FishingWeb.CompetitorsLive.Index do
  use FishingWeb, :live_view

  alias Fishing.Registration
  alias Fishing.Registration.Competitors

  @impl true
  def mount(_params, _session, socket) do
    {:ok, stream(socket, :competitor, Registration.list_competitor())}
  end

  @impl true
  def handle_params(params, _url, socket) do
    {:noreply, apply_action(socket, socket.assigns.live_action, params)}
  end

  defp apply_action(socket, :edit, %{"id" => id}) do
    socket
    |> assign(:page_title, "Edit Competitors")
    |> assign(:competitors, Registration.get_competitors!(id))
  end

  defp apply_action(socket, :new, _params) do
    socket
    |> assign(:page_title, "New Competitors")
    |> assign(:competitors, %Competitors{})
  end

  defp apply_action(socket, :index, _params) do
    socket
    |> assign(:page_title, "Listing Competitor")
    |> assign(:competitors, nil)
  end

  @impl true
  def handle_info({FishingWeb.CompetitorsLive.FormComponent, {:saved, competitors}}, socket) do
    {:noreply, stream_insert(socket, :competitor, competitors)}
  end

  @impl true
  def handle_event("delete", %{"id" => id}, socket) do
    competitors = Registration.get_competitors!(id)
    {:ok, _} = Registration.delete_competitors(competitors)

    {:noreply, stream_delete(socket, :competitor, competitors)}
  end
end
